// Log Viewer - Online Mode JavaScript

// Global state
let mappings = {};
let currentKey = null;
let currentImages = [];
let currentImageIndex = 0;
let remoteLogs = {};
const serveMode = true;
let connectMode = 'cluster';

function appBasePath() {
  const pathname = window.location.pathname || '/';
  if (pathname.endsWith('/')) return pathname;
  const lastSlash = pathname.lastIndexOf('/');
  return pathname.slice(0, lastSlash + 1);
}

function apiPath(path) {
  const clean = String(path || '').replace(/^\/+/, '');
  return appBasePath() + clean;
}

// Initialize on DOM load
document.addEventListener('DOMContentLoaded', async function() {
  await loadMappings();
  initializeUI();
  setupEventListeners();
});

// Load mappings from server
async function loadMappings() {
  try {
    const resp = await fetch(apiPath('api/mappings'));
    const data = await resp.json();
    if (data.success) {
      mappings = data.mappings || {};
      populateLogDropdown();
    }
  } catch (e) {
    console.error('Failed to load mappings:', e);
  }
}

// Populate log dropdown
function populateLogDropdown() {
  const select = document.getElementById('logSelect');
  select.innerHTML = '';
  
  const keys = Object.keys(mappings).sort();
  if (keys.length === 0) {
    const opt = document.createElement('option');
    opt.value = '';
    opt.textContent = 'No data available';
    select.appendChild(opt);
    return;
  }
  
  keys.forEach(key => {
    const opt = document.createElement('option');
    opt.value = key;
    opt.textContent = mappings[key].html_folder || key;
    select.appendChild(opt);
  });
  
  // Select first and update
  if (keys.length > 0) {
    updateScenario(keys[0]);
  }
}

// Initialize UI elements
function initializeUI() {
  setupExpandButtons();
  setupImageModal();
}

// Setup event listeners
function setupEventListeners() {
  // Log selection
  document.getElementById('logSelect').addEventListener('change', function() {
    updateScenario(this.value);
  });
  
  // Sensor filter
  document.getElementById('sensorFilter').addEventListener('change', function() {
    const entry = mappings[currentKey];
    if (entry) {
      buildSensorGrid(entry, this.value);
    }
  });
  
  // Edit comment button
  document.getElementById('editCommentBtn').addEventListener('click', function() {
    const box = document.getElementById('commentBox');
    box.readOnly = false;
    box.focus();
  });
  
  // Save comment button
  document.getElementById('saveCommentBtn').addEventListener('click', saveComment);
  
  // Open HTML button
  document.getElementById('openHtmlBtn').addEventListener('click', openHtmlPreview);
  
  // Go Online button
  document.getElementById('goOnlineBtn').addEventListener('click', openClusterModal);
}

// Update scenario when log selected
async function updateScenario(key) {
  currentKey = key;
  const entry = mappings[key];
  
  if (!entry) {
    document.getElementById('logNameDisplay').textContent = '';
    document.getElementById('videoPlayer').src = '';
    document.getElementById('videoInfo').textContent = 'No video';
    document.getElementById('commentBox').value = '';
    document.getElementById('sensorGrid').innerHTML = '<div class="no-data">No data</div>';
    return;
  }
  
  // Update log name display
  document.getElementById('logNameDisplay').textContent = entry.html_folder || key;
  
  // Update video
  const video = document.getElementById('videoPlayer');
  video.src = entry.video;
  document.getElementById('videoInfo').textContent = entry.video_name;
  
  // Update sensor filter
  updateSensorFilter(entry);
  
  // Build sensor grid
  buildSensorGrid(entry, 'all');
  
  // Load existing comment
  await loadExistingComment(entry);
}

// Update sensor filter dropdown
function updateSensorFilter(entry) {
  const select = document.getElementById('sensorFilter');
  select.innerHTML = '<option value="all">All</option>';
  
  if (entry && entry.images) {
    Object.keys(entry.images).sort().forEach(sensor => {
      const opt = document.createElement('option');
      opt.value = sensor;
      opt.textContent = sensor.charAt(0).toUpperCase() + sensor.slice(1);
      select.appendChild(opt);
    });
  }
}

// Build sensor grid
function buildSensorGrid(entry, filter) {
  const grid = document.getElementById('sensorGrid');
  grid.innerHTML = '';
  currentImages = [];
  
  if (!entry || !entry.images) {
    grid.innerHTML = '<div class="no-data">No images available</div>';
    return;
  }
  
  const images = entry.images;
  const sensors = filter === 'all' ? Object.keys(images) : [filter];
  
  sensors.forEach(sensor => {
    if (!images[sensor]) return;
    
    Object.entries(images[sensor]).forEach(([position, imgPath]) => {
      currentImages.push({
        path: imgPath,
        sensor: sensor,
        position: position
      });
      
      const card = document.createElement('div');
      card.className = 'sensor-card';
      card.onclick = () => openImageModal(currentImages.length - 1);
      
      const img = document.createElement('img');
      img.src = imgPath;
      img.alt = `${sensor} - ${position}`;
      img.loading = 'lazy';
      
      const label = document.createElement('div');
      label.className = 'sensor-card-label';
      label.textContent = `${sensor.toUpperCase()} - ${position}`;
      
      card.appendChild(img);
      card.appendChild(label);
      grid.appendChild(card);
    });
  });
  
  if (currentImages.length === 0) {
    grid.innerHTML = '<div class="no-data">No images for this filter</div>';
  } else {
    // Set data-count attribute for adaptive grid layout
    grid.setAttribute('data-count', currentImages.length.toString());
  }
}

// Load existing comment
async function loadExistingComment(entry) {
  const commentBox = document.getElementById('commentBox');
  const commentPath = document.getElementById('commentPath');
  
  if (!entry) {
    commentBox.value = '';
    commentPath.textContent = '';
    return;
  }
  
  commentPath.textContent = entry.comment_path || '';
  
  try {
    const resp = await fetch(apiPath('api/get-comment'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ path: entry.comment_path })
    });
    const data = await resp.json();
    commentBox.value = data.success ? (data.content || '') : (entry.comment_content || '');
  } catch (e) {
    commentBox.value = entry.comment_content || '';
  }
  
  commentBox.readOnly = true;
}

// Save comment
async function saveComment() {
  const entry = mappings[currentKey];
  if (!entry) return;
  
  const commentBox = document.getElementById('commentBox');
  const content = commentBox.value;
  
  try {
    const resp = await fetch(apiPath('api/save-comment'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        path: entry.comment_path,
        content: content
      })
    });
    const data = await resp.json();
    
    if (data.success) {
      commentBox.readOnly = true;
      showNotification('Comment saved!', 'success');
    } else {
      showNotification('Failed to save: ' + data.error, 'error');
    }
  } catch (e) {
    showNotification('Error saving comment', 'error');
  }
}

// Open HTML preview
function openHtmlPreview() {
  const entry = mappings[currentKey];
  if (!entry || !entry.html_files || entry.html_files.length === 0) {
    showNotification('No HTML files available', 'error');
    return;
  }
  
  // Open first HTML file in new tab
  const htmlFile = entry.html_files[0];
  window.open(htmlFile, '_blank');
}

// Setup expand buttons
function setupExpandButtons() {
  document.querySelectorAll('.expand-btn').forEach(btn => {
    btn.addEventListener('click', function(e) {
      e.stopPropagation();
      const box = this.closest('.box');
      box.classList.toggle('expanded');
      this.textContent = box.classList.contains('expanded') ? '✕' : '⛶';
    });
  });
}

// Image Modal functions
function setupImageModal() {
  const modal = document.getElementById('imageModal');
  
  modal.querySelector('.image-modal-close').addEventListener('click', closeImageModal);
  modal.querySelector('.image-modal-nav.prev').addEventListener('click', () => navigateModal(-1));
  modal.querySelector('.image-modal-nav.next').addEventListener('click', () => navigateModal(1));
  
  modal.addEventListener('click', function(e) {
    if (e.target === this) closeImageModal();
  });
  
  document.addEventListener('keydown', function(e) {
    if (!modal.classList.contains('active')) return;
    
    if (e.key === 'Escape') closeImageModal();
    else if (e.key === 'ArrowLeft') navigateModal(-1);
    else if (e.key === 'ArrowRight') navigateModal(1);
  });
}

function openImageModal(index) {
  currentImageIndex = index;
  const modal = document.getElementById('imageModal');
  const img = modal.querySelector('img');
  const info = modal.querySelector('.image-modal-info');
  
  const imageData = currentImages[index];
  img.src = imageData.path;
  info.textContent = `${imageData.sensor.toUpperCase()} - ${imageData.position} (${index + 1}/${currentImages.length})`;
  
  modal.classList.add('active');
}

function closeImageModal() {
  document.getElementById('imageModal').classList.remove('active');
}

function navigateModal(direction) {
  currentImageIndex = (currentImageIndex + direction + currentImages.length) % currentImages.length;
  const modal = document.getElementById('imageModal');
  const img = modal.querySelector('img');
  const info = modal.querySelector('.image-modal-info');
  
  const imageData = currentImages[currentImageIndex];
  img.src = imageData.path;
  info.textContent = `${imageData.sensor.toUpperCase()} - ${imageData.position} (${currentImageIndex + 1}/${currentImages.length})`;
}

// ============================================
// Cluster Connection Functions
// ============================================

function openClusterModal() {
  document.getElementById('clusterModal').classList.add('active');
  setConnectMode(connectMode);
  checkClusterStatus();
}

function closeClusterModal() {
  document.getElementById('clusterModal').classList.remove('active');
}

async function checkClusterStatus() {
  try {
    const resp = await fetch(apiPath('api/cluster/status'));
    const data = await resp.json();
    updateClusterUI(Boolean(data.connected), data.server || null);

    // Optionally prefill last-used values
    if (data && typeof data === 'object') {
      const htmlEl = document.getElementById('remoteHtmlPathInput');
      const videoEl = document.getElementById('remoteVideoPathInput');
      const outEl = document.getElementById('outputPathInput');
      const netIdEl = document.getElementById('netIdInput');
      if (htmlEl && !htmlEl.value && data.html_path) htmlEl.value = data.html_path;
      if (videoEl && !videoEl.value && data.video_path) videoEl.value = data.video_path;
      if (outEl && !outEl.value && data.output_root) outEl.value = data.output_root;
      if (netIdEl && !netIdEl.value && data.net_id) netIdEl.value = data.net_id;
    }
  } catch (e) {
    console.error('Failed to check cluster status:', e);
  }
}

function updateClusterUI(connected, serverName) {
  const statusDot = document.getElementById('statusDot');
  const statusText = document.getElementById('statusText');
  const clusterPanel = document.getElementById('clusterPanel');
  const localPanel = document.getElementById('localPanel');
  const passwordGroup = document.getElementById('passwordGroup');
  const netIdGroup = document.getElementById('netIdGroup');
  
  statusDot.className = 'status-dot ' + (connected ? 'connected' : 'disconnected');
  statusText.textContent = connected ? `Connected to ${serverName}` : 'Disconnected';
  
  // Cluster panels are only relevant in cluster mode.
  if (connectMode !== 'cluster') {
    clusterPanel.classList.remove('active');
    localPanel.classList.add('active');
    return;
  }

  // Show appropriate cluster panel
  localPanel.classList.remove('active');
  clusterPanel.classList.add('active');
  if (passwordGroup) {
    passwordGroup.style.display = connected ? 'none' : '';
  }
  if (netIdGroup) {
    netIdGroup.style.display = connected ? 'none' : '';
  }
}

function setConnectMode(mode) {
  connectMode = mode === 'local' ? 'local' : 'cluster';
  const modeClusterBtn = document.getElementById('modeClusterBtn');
  const modeLocalBtn = document.getElementById('modeLocalBtn');
  const clusterPanel = document.getElementById('clusterPanel');
  const localPanel = document.getElementById('localPanel');

  modeClusterBtn.classList.toggle('active', connectMode === 'cluster');
  modeLocalBtn.classList.toggle('active', connectMode === 'local');

  // Default view; cluster status check will refine for connected/disconnected.
  if (connectMode === 'local') {
    clusterPanel.classList.remove('active');
    localPanel.classList.add('active');
  } else {
    localPanel.classList.remove('active');
    clusterPanel.classList.add('active');
    // Let checkClusterStatus()/updateClusterUI decide connected/disconnected details.
  }
}

// Toggle password visibility
function togglePassword() {
  const input = document.getElementById('passwordInput');
  const btn = document.querySelector('.toggle-password');
  
  if (input.type === 'password') {
    input.type = 'text';
    btn.textContent = '🙈';
  } else {
    input.type = 'password';
    btn.textContent = '👁';
  }
}

function togglePasswordConfirm() {
  // Removed (single password confirmation field only)
}

async function clusterConnect() {
  const htmlPath = document.getElementById('remoteHtmlPathInput').value.trim();
  const videoPath = document.getElementById('remoteVideoPathInput').value.trim();
  const outputPath = (document.getElementById('outputPathInput')?.value || '').trim();
  const netId = (document.getElementById('netIdInput')?.value || '').trim();
  const password = document.getElementById('passwordInput').value;
  
  if (!htmlPath || !videoPath) {
    showClusterMessage('Please enter both HTML and Video paths', 'error');
    return false;
  }

  if (!netId) {
    showClusterMessage('Please enter your Net ID', 'error');
    return false;
  }

  if (!password) {
    showClusterMessage('Please enter your password', 'error');
    return false;
  }
  
  showLoading('Connecting...');
  
  try {
    const resp = await fetch(apiPath('api/cluster/connect'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ html_path: htmlPath, video_path: videoPath, output_path: outputPath, net_id: netId, password })
    });
    const data = await resp.json();
    
    hideLoading();
    
    if (data.success) {
      showClusterMessage('Connected successfully!', 'success');
      updateClusterUI(true, data.server || data.cluster || 'cluster');
      // Best-effort: clear password after successful connect
      const passEl = document.getElementById('passwordInput');
      if (passEl) passEl.value = '';
      return true;
    } else {
      showClusterMessage(data.error || 'Connection failed', 'error');
      return false;
    }
  } catch (e) {
    hideLoading();
    showClusterMessage('Connection error: ' + e.message, 'error');
    return false;
  }
}

async function ensureClusterConnected() {
  // First check if already connected
  try {
    const resp = await fetch(apiPath('api/cluster/status'));
    const data = await resp.json();
    if (data && data.connected) {
      updateClusterUI(true, data.server || 'cluster');
      return true;
    }
  } catch (e) {
    // ignore and attempt connect
  }
  
  // Not connected - check if credentials are available
  const netId = (document.getElementById('netIdInput')?.value || '').trim();
  const password = document.getElementById('passwordInput')?.value || '';
  
  if (!netId || !password) {
    showClusterMessage('Please enter your Net ID and password, then click Connect', 'error');
    return false;
  }
  
  // Attempt connection with available credentials
  const connected = await clusterConnect();
  return connected === true;
  } 


async function clusterDisconnect() {
  try {
    await fetch(apiPath('api/cluster/disconnect'), { method: 'POST' });
    updateClusterUI(false, null);
    showClusterMessage('Disconnected', 'info');
    
    // Clear remote logs
    document.getElementById('remoteLogsSection').style.display = 'none';
    document.getElementById('remoteLogsList').innerHTML = '';
    remoteLogs = {};
  } catch (e) {
    console.error('Disconnect error:', e);
  }
}

// Scan remote logs from the given path
async function scanRemoteLogs() {
  const htmlPath = document.getElementById('remoteHtmlPathInput').value.trim();
  const videoPath = document.getElementById('remoteVideoPathInput').value.trim();
  const outputPath = (document.getElementById('outputPathInput')?.value || '').trim();
  
  if (!htmlPath || !videoPath) {
    showClusterMessage('Please enter both HTML and Video paths', 'error');
    return;
  }

  const connected = await ensureClusterConnected();
  if (!connected) {
    showClusterMessage('Not connected', 'error');
    return;
  }
  
  showLoading('Scanning remote logs...');
  
  try {
    const resp = await fetch(apiPath('api/cluster/scan-logs'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ html_path: htmlPath, video_path: videoPath, output_path: outputPath })
    });
    const data = await resp.json();
    
    hideLoading();
    
    if (data.success) {
      remoteLogs = data.logs || {};
      displayRemoteLogs();
      showClusterMessage(`Found ${Object.keys(remoteLogs).length} logs`, 'success');
    } else {
      showClusterMessage(data.error || 'Scan failed', 'error');
    }
  } catch (e) {
    hideLoading();
    showClusterMessage('Scan error: ' + e.message, 'error');
  }
}

function displayRemoteLogs() {
  const section = document.getElementById('remoteLogsSection');
  const list = document.getElementById('remoteLogsList');
  
  list.innerHTML = '';
  
  const logKeys = Object.keys(remoteLogs).sort();
  
  if (logKeys.length === 0) {
    section.style.display = 'none';
    return;
  }
  
  section.style.display = 'block';
  
  logKeys.forEach(key => {
    const log = remoteLogs[key];
    const isCached = log.cached === true;
    const item = document.createElement('div');
    item.className = 'remote-log-item' + (isCached ? ' cached' : '');
    item.id = 'log-' + key;
    item.onclick = () => downloadLog(key);
    
    const icon = isCached ? '✅' : '📦';
    const status = isCached ? 'Cached - Click to view' : 'Click to download';
    
    item.innerHTML = `
      <span class="remote-log-icon">${icon}</span>
      <span class="remote-log-name">${log.name || key}</span>
      <span class="remote-log-status">${status}</span>
    `;
    
    list.appendChild(item);
  });
}

async function downloadLog(key) {
  const log = remoteLogs[key];
  if (!log) return;

  const outputPath = (document.getElementById('outputPathInput')?.value || '').trim();
  
  const item = document.getElementById('log-' + key);
  
  // If already cached or downloaded, just load it
  if (item.classList.contains('cached') || item.classList.contains('downloaded')) {
    closeClusterModal();
    await loadMappings();
    updateScenario(key);
    return;
  }
  
  if (item.classList.contains('downloading')) {
    return;
  }
  
  item.classList.add('downloading');
  item.querySelector('.remote-log-status').textContent = 'Downloading...';
  
  showLoading('Downloading ' + (log.name || key) + '...');
  
  try {
    const resp = await fetch(apiPath('api/cluster/download-log'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        key: key,
        html_path: log.html_path,
        video_path: log.video_path,
        output_path: outputPath
      })
    });
    const data = await resp.json();
    
    hideLoading();
    
    if (data.success) {
      item.classList.remove('downloading');
      item.classList.add('downloaded');
      item.querySelector('.remote-log-status').textContent = '✓ Downloaded';
      item.querySelector('.remote-log-icon').textContent = '✅';
      
      showClusterMessage('Downloaded successfully! Click again to view.', 'success');
      
      // Reload mappings
      await loadMappings();
    } else {
      item.classList.remove('downloading');
      item.querySelector('.remote-log-status').textContent = 'Failed';
      showClusterMessage(data.error || 'Download failed', 'error');
    }
  } catch (e) {
    hideLoading();
    item.classList.remove('downloading');
    item.querySelector('.remote-log-status').textContent = 'Error';
    showClusterMessage('Download error: ' + e.message, 'error');
  }
}

async function localUpload() {
  const htmlInput = document.getElementById('localHtmlFolderInput');
  const videoInput = document.getElementById('localVideoFilesInput');
  const outputPath = (document.getElementById('localOutputPathInput')?.value || '').trim();

  const htmlFiles = Array.from(htmlInput?.files || []);
  const videoFiles = Array.from(videoInput?.files || []);

  if (htmlFiles.length === 0 && videoFiles.length === 0) {
    showClusterMessage('Please select HTML folder and/or video files', 'error');
    return;
  }

  const fd = new FormData();
  fd.append('output_path', outputPath);

  htmlFiles.forEach(f => {
    const rel = f.webkitRelativePath || f.name;
    fd.append('html_files', f, rel);
  });
  videoFiles.forEach(f => {
    fd.append('video_files', f, f.name);
  });

  showLoading('Uploading files to server...');
  try {
    const resp = await fetch(apiPath('api/local/upload'), { method: 'POST', body: fd });
    const data = await resp.json();
    hideLoading();
    if (data.success) {
      showClusterMessage(`Upload complete (HTML: ${data.saved_html}, Video: ${data.saved_video})`, 'success');
      closeClusterModal();
      await loadMappings();
    } else {
      showClusterMessage(data.error || 'Upload failed', 'error');
    }
  } catch (e) {
    hideLoading();
    showClusterMessage('Upload error: ' + e.message, 'error');
  }
}

// Loading helpers
function showLoading(text) {
  const overlay = document.getElementById('loadingOverlay');
  document.getElementById('loadingText').textContent = text || 'Loading...';
  overlay.style.display = 'flex';
}

function hideLoading() {
  document.getElementById('loadingOverlay').style.display = 'none';
}

function showClusterMessage(msg, type) {
  const el = document.getElementById('clusterMessage');
  el.textContent = msg;
  el.className = 'cluster-message ' + type;
  el.style.display = 'block';
  
  setTimeout(() => {
    el.style.display = 'none';
  }, 5000);
}

// Notification helper
function showNotification(message, type) {
  if (type === 'error') {
    alert('Error: ' + message);
  } else {
    alert(message);
  }
}
