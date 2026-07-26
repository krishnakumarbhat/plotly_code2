// Hyperlink Stream JavaScript

let mappings = {};
let currentKey = null;
let currentImages = [];
let currentImageIndex = 0;
let remoteLogs = {};
let vlmInFlight = false;
let vlmEnabled = true;
let vlmModelDir = '';
let connectMode = 'cluster';
let pendingClusterAction = null;
let savedRailOpen = true;
let toastTimer = null;
let clusterStatusState = null;
let clusterStatusRequest = null;
const videoLoadState = {
  primary: { token: 0, blobUrl: null },
  overlay: { token: 0, blobUrl: null },
};
const SAVED_RAIL_WIDTH_KEY = 'hyperlink.savedRailWidth';
const SAVED_RAIL_DEFAULT_WIDTH = 320;
const SAVED_RAIL_MIN_WIDTH = 320;
const SAVED_RAIL_MAX_WIDTH = 780;
let savedRailResizeState = null;

function appBasePath() {
  const pathname = window.location.pathname || '/';
  if (pathname.endsWith('/')) {
    return pathname;
  }
  const lastSlash = pathname.lastIndexOf('/');
  return pathname.slice(0, lastSlash + 1);
}

function apiPath(path) {
  const clean = String(path || '').replace(/^\/+/, '');
  return appBasePath() + clean;
}

function getVisibleLabel(entry, key) {
  if (!entry) {
    return key || 'Untitled log';
  }
  return entry.saved_label || entry.html_folder || entry.video_name || key || 'Untitled log';
}

function getMappingEntries() {
  return Object.entries(mappings).sort(function (left, right) {
    const leftEntry = left[1] || {};
    const rightEntry = right[1] || {};
    const leftStamp = Date.parse(leftEntry.last_used_at || leftEntry.saved_at || '') || 0;
    const rightStamp = Date.parse(rightEntry.last_used_at || rightEntry.saved_at || '') || 0;
    if (leftStamp !== rightStamp) {
      return rightStamp - leftStamp;
    }
    return getVisibleLabel(leftEntry, left[0]).localeCompare(getVisibleLabel(rightEntry, right[0]));
  });
}

document.addEventListener('DOMContentLoaded', async function () {
  initializeUI();
  setupEventListeners();
  await loadVlmStatus();
  await loadMappings();
  await checkClusterStatus();
});

async function loadMappings() {
  try {
    const resp = await fetch(apiPath('api/mappings'));
    const data = await resp.json();
    if (data.success) {
      mappings = data.mappings || {};
    } else {
      mappings = {};
    }
  } catch (error) {
    console.error('Failed to load mappings:', error);
    mappings = {};
  }

  const keys = Object.keys(mappings);
  if (keys.length === 0) {
    await updateScenario(null);
    return;
  }

  if (!currentKey || !mappings[currentKey]) {
    currentKey = keys[0];
  }
  await updateScenario(currentKey);
}

async function loadVlmStatus() {
  try {
    const resp = await fetch(apiPath('api/vlm/status'));
    const data = await resp.json();
    vlmEnabled = Boolean(data.enabled);
    vlmModelDir = data.model_dir || '';
    setVlmBusy(false);

    if (!vlmEnabled) {
      setVlmStatus('AI video description is disabled for this session.', 'error');
      return;
    }
    if (vlmModelDir) {
      setVlmStatus('Offline model ready: ' + vlmModelDir, 'success');
    } else {
      setVlmStatus('AI video description is enabled, but no local model path is configured yet.', 'error');
    }
  } catch (error) {
    vlmEnabled = false;
    setVlmBusy(false);
    setVlmStatus('Unable to load AI status: ' + error.message, 'error');
  }
}

function initializeUI() {
  savedRailOpen = window.innerWidth > 1180;
  initializeSavedRailWidth();
  setSavedRailOpen(savedRailOpen);
  setupExpandButtons();
  setupImageModal();
}

function setupEventListeners() {
  document.getElementById('savedRailToggle').addEventListener('click', function () {
    setSavedRailOpen(!savedRailOpen);
  });
  document.getElementById('savedRailClose').addEventListener('click', function () {
    setSavedRailOpen(false);
  });
  document.getElementById('savedSearchInput').addEventListener('input', renderSavedList);
  setupSavedRailResizer();
  window.addEventListener('resize', handleViewportResize);

  document.getElementById('sensorFilter').addEventListener('change', function () {
    const entry = mappings[currentKey];
    if (entry) {
      buildSensorGrid(entry, this.value);
    }
  });

  document.getElementById('editCommentBtn').addEventListener('click', function () {
    const box = document.getElementById('commentBox');
    box.readOnly = false;
    box.focus();
  });
  document.getElementById('saveCommentBtn').addEventListener('click', saveComment);
  document.getElementById('generateVlmBtn').addEventListener('click', function () {
    generateVlmDescription(false);
  });
  document.getElementById('openHtmlBtn').addEventListener('click', openHtmlPreview);
  document.getElementById('goOnlineBtn').addEventListener('click', openClusterModal);

  document.getElementById('clusterModalClose').addEventListener('click', closeClusterModal);
  document.getElementById('modeStreamBtn').addEventListener('click', function () {
    setConnectMode('cluster');
  });
  document.getElementById('modeLocalBtn').addEventListener('click', function () {
    setConnectMode('local');
  });
  document.getElementById('clusterScanBtn').addEventListener('click', function () {
    scanRemoteLogs(false);
  });
  document.getElementById('clusterResetBtn').addEventListener('click', clusterDisconnect);
  document.getElementById('togglePasswordBtn').addEventListener('click', togglePassword);
  document.getElementById('clusterAuthBtn').addEventListener('click', clusterConnect);
  document.getElementById('clusterAuthCancelBtn').addEventListener('click', function () {
    hideClusterAuth(true);
  });
  document.getElementById('localUploadBtn').addEventListener('click', localUpload);

  document.getElementById('clusterModal').addEventListener('click', function (event) {
    if (event.target === this) {
      closeClusterModal();
    }
  });
}

function setSavedRailOpen(isOpen) {
  savedRailOpen = Boolean(isOpen);
  const rail = document.getElementById('savedRail');
  const toggle = document.getElementById('savedRailToggle');
  rail.classList.toggle('collapsed', !savedRailOpen);
  toggle.setAttribute('aria-expanded', savedRailOpen ? 'true' : 'false');
  if (savedRailOpen && window.innerWidth > 900) {
    initializeSavedRailWidth();
  }
}

function clampSavedRailWidth(width) {
  const viewportLimit = Math.max(SAVED_RAIL_MIN_WIDTH, window.innerWidth - 180);
  return Math.max(SAVED_RAIL_MIN_WIDTH, Math.min(SAVED_RAIL_MAX_WIDTH, Math.min(viewportLimit, Math.round(width))));
}

function savedRailWidthRoot() {
  return document.querySelector('.page') || document.documentElement;
}

function applySavedRailWidth(width, persist) {
  const root = savedRailWidthRoot();
  if (window.innerWidth <= 900) {
    root.style.removeProperty('--saved-rail-width');
    return;
  }
  const bounded = clampSavedRailWidth(width);
  root.style.setProperty('--saved-rail-width', bounded + 'px');
  if (persist) {
    window.localStorage.setItem(SAVED_RAIL_WIDTH_KEY, String(bounded));
  }
}

function initializeSavedRailWidth() {
  const root = savedRailWidthRoot();
  if (window.innerWidth <= 900) {
    root.style.removeProperty('--saved-rail-width');
    return;
  }
  const storedWidth = Number(window.localStorage.getItem(SAVED_RAIL_WIDTH_KEY) || SAVED_RAIL_DEFAULT_WIDTH);
  applySavedRailWidth(Number.isFinite(storedWidth) ? storedWidth : SAVED_RAIL_DEFAULT_WIDTH, false);
}

function endSavedRailResize() {
  if (!savedRailResizeState) {
    return;
  }
  const rail = document.getElementById('savedRail');
  if (rail && !rail.classList.contains('collapsed') && window.innerWidth > 900) {
    applySavedRailWidth(rail.getBoundingClientRect().width, true);
  }
  savedRailResizeState = null;
  document.body.classList.remove('saved-rail-resizing');
}

function handleViewportResize() {
  const root = savedRailWidthRoot();
  if (window.innerWidth <= 900) {
    endSavedRailResize();
    root.style.removeProperty('--saved-rail-width');
    return;
  }
  initializeSavedRailWidth();
}

function setupSavedRailResizer() {
  const handle = document.getElementById('savedRailResizeHandle');
  if (!handle) {
    return;
  }

  handle.addEventListener('pointerdown', function (event) {
    const rail = document.getElementById('savedRail');
    if (!rail || rail.classList.contains('collapsed') || window.innerWidth <= 900) {
      return;
    }
    savedRailResizeState = {
      startX: event.clientX,
      startWidth: rail.getBoundingClientRect().width,
    };
    document.body.classList.add('saved-rail-resizing');
    event.preventDefault();
  });

  window.addEventListener('pointermove', function (event) {
    if (!savedRailResizeState) {
      return;
    }
    applySavedRailWidth(savedRailResizeState.startWidth + (event.clientX - savedRailResizeState.startX), false);
  });

  window.addEventListener('pointerup', endSavedRailResize);
  window.addEventListener('pointercancel', endSavedRailResize);

  handle.addEventListener('keydown', function (event) {
    const rail = document.getElementById('savedRail');
    if (!rail || rail.classList.contains('collapsed') || window.innerWidth <= 900) {
      return;
    }
    if (event.key !== 'ArrowLeft' && event.key !== 'ArrowRight') {
      return;
    }
    const delta = event.key === 'ArrowLeft' ? -24 : 24;
    applySavedRailWidth(rail.getBoundingClientRect().width + delta, true);
    event.preventDefault();
  });
}

function renderSavedList() {
  const list = document.getElementById('savedList');
  const query = (document.getElementById('savedSearchInput').value || '').trim().toLowerCase();
  list.innerHTML = '';

  const entries = getMappingEntries().filter(function (item) {
    const key = item[0];
    const entry = item[1] || {};
    const haystack = [
      getVisibleLabel(entry, key),
      entry.video_name || '',
      entry.overlay_video_name || '',
      entry.remote_html_path || '',
      entry.remote_video_path || '',
      entry.remote_overlay_path || '',
    ].join(' ').toLowerCase();
    return !query || haystack.indexOf(query) !== -1;
  });

  if (entries.length === 0) {
    const empty = document.createElement('div');
    empty.className = 'saved-empty';
    empty.textContent = query ? 'No logs match this search.' : 'No saved logs yet.';
    list.appendChild(empty);
    return;
  }

  entries.forEach(function (item) {
    const key = item[0];
    const entry = item[1] || {};
    const row = document.createElement('div');
    row.className = 'saved-item' + (key === currentKey ? ' active' : '');

    const main = document.createElement('button');
    main.type = 'button';
    main.className = 'saved-item-main';
    main.addEventListener('click', async function () {
      await updateScenario(key);
      if (window.innerWidth < 1180) {
        setSavedRailOpen(false);
      }
    });

    const title = document.createElement('div');
    title.className = 'saved-item-title';
    title.textContent = getVisibleLabel(entry, key);
    title.title = title.textContent;

    const meta = document.createElement('div');
    meta.className = 'saved-item-meta';
    const metaParts = [entry.video_name || entry.remote_video_path || 'Cached locally'];
    if (entry.has_overlay || entry.overlay_video_name || entry.remote_overlay_path) {
      metaParts.push('Overlay: ' + (entry.overlay_video_name || entry.remote_overlay_path || 'attached'));
    }
    meta.textContent = metaParts.join(' • ');
    meta.title = meta.textContent;

    const badge = document.createElement('span');
    badge.className = 'saved-item-badge' + (entry.saved_id ? ' saved' : ' local');
    badge.textContent = entry.saved_id ? 'Saved' : 'Local';

    main.appendChild(title);
    main.appendChild(meta);
    row.appendChild(main);
    row.appendChild(badge);

    if (entry.saved_id) {
      const actions = document.createElement('div');
      actions.className = 'saved-item-actions';

      const overlayBtn = document.createElement('button');
      overlayBtn.type = 'button';
      overlayBtn.className = 'saved-item-overlay';
      overlayBtn.textContent = entry.has_overlay ? 'Replace Overlay' : 'Attach Overlay';
      overlayBtn.addEventListener('click', async function (event) {
        event.stopPropagation();
        await attachOverlay(entry.saved_id, entry);
      });
      actions.appendChild(overlayBtn);

      const deleteBtn = document.createElement('button');
      deleteBtn.type = 'button';
      deleteBtn.className = 'saved-item-delete';
      deleteBtn.textContent = 'Delete';
      deleteBtn.addEventListener('click', async function (event) {
        event.stopPropagation();
        await deleteSavedLog(entry.saved_id);
      });
      actions.appendChild(deleteBtn);
      row.appendChild(actions);
    }

    list.appendChild(row);
  });
}

function getVideoState(slot) {
  return slot === 'overlay' ? videoLoadState.overlay : videoLoadState.primary;
}

function clearVideoBlobUrl(slot) {
  const state = getVideoState(slot);
  if (state.blobUrl) {
    URL.revokeObjectURL(state.blobUrl);
    state.blobUrl = null;
  }
}

function resetVideoPlayer(video, infoId, message, slot) {
  const state = getVideoState(slot);
  state.token += 1;
  clearVideoBlobUrl(slot);
  video.pause();
  video.removeAttribute('src');
  video.load();
  const info = document.getElementById(infoId);
  if (info) {
    info.textContent = message;
  }
}

function setOverlayPanelVisible(isVisible) {
  const stage = document.getElementById('videoStage');
  const panel = document.getElementById('overlayVideoPanel');
  if (!stage || !panel) {
    return;
  }
  stage.classList.toggle('has-overlay', Boolean(isVisible));
  panel.classList.toggle('hidden', !isVisible);
}

function buildVideoChunkUrl(url, start, size) {
  const chunkBase = url
    .replace('/hyperlink/data/video/', '/hyperlink/api/video-chunk/')
    .replace('/data/video/', '/hyperlink/api/video-chunk/');
  const separator = chunkBase.includes('?') ? '&' : '?';
  return chunkBase + separator + 'start=' + start + '&size=' + size;
}

async function fetchVideoBlob(url, onProgress, slot, loadToken) {
  const state = getVideoState(slot);
  const chunks = [];
  const chunkSize = 16 * 1024;
  let downloaded = 0;
  let expectedSize = null;
  let contentType = 'video/mp4';

  while (expectedSize === null || downloaded < expectedSize) {
    const response = await fetch(buildVideoChunkUrl(url, downloaded, chunkSize), {
      credentials: 'same-origin',
    });

    if (!response.ok) {
      throw new Error('Video download failed with status ' + response.status);
    }

    expectedSize = Number(response.headers.get('x-file-size')) || expectedSize;

    const buffer = await response.arrayBuffer();
    if (loadToken !== state.token) {
      return null;
    }

    const byteLength = buffer.byteLength;
    if (!byteLength) {
      break;
    }

    chunks.push(buffer);
    downloaded += byteLength;

    if (typeof onProgress === 'function') {
      onProgress(downloaded, expectedSize);
    }

    if (byteLength < chunkSize) {
      expectedSize = expectedSize || downloaded;
      break;
    }
  }

  return new Blob(chunks, { type: contentType });
}

async function loadVideoIntoPlayer(video, entry, label, options) {
  const slot = (options && options.slot) || 'primary';
  const infoId = (options && options.infoId) || 'videoInfo';
  const urlKey = (options && options.urlKey) || 'video';
  const nameKey = (options && options.nameKey) || 'video_name';
  const state = getVideoState(slot);
  const loadToken = ++state.token;
  clearVideoBlobUrl(slot);
  video.pause();
  video.removeAttribute('src');
  video.load();

  const videoInfo = document.getElementById(infoId);
  videoInfo.textContent = 'Loading video...';

  try {
    const blob = await fetchVideoBlob(entry[urlKey], function (downloaded, expectedSize) {
      if (loadToken !== state.token) {
        return;
      }
      if (expectedSize) {
        const percent = Math.max(1, Math.min(100, Math.round((downloaded / expectedSize) * 100)));
        videoInfo.textContent = 'Loading video... ' + percent + '%';
      }
    }, slot, loadToken);

    if (loadToken !== state.token || !blob) {
      return;
    }

    state.blobUrl = URL.createObjectURL(blob);
    video.src = state.blobUrl;
    video.preload = 'auto';
    video.load();
    videoInfo.textContent = entry[nameKey] || label;
  } catch (error) {
    if (loadToken !== state.token) {
      return;
    }
    console.error('Video blob preload failed:', error);
    video.src = entry[urlKey];
    video.preload = 'metadata';
    video.load();
    videoInfo.textContent = 'Loading video...';
  }
}

async function deleteSavedLog(savedId) {
  try {
    const resp = await fetch(apiPath('api/saved-logs/' + savedId), { method: 'DELETE' });
    const data = await resp.json();
    if (!data.success) {
      throw new Error(data.error || 'Delete failed');
    }
    if (String(savedId) === String(currentKey)) {
      currentKey = null;
    }
    await loadMappings();
    showNotification('Removed from saved logs.', 'success');
  } catch (error) {
    showNotification('Delete failed: ' + error.message, 'error');
  }
}

function suggestOverlayPath(entry) {
  if (entry && entry.remote_overlay_path) {
    return entry.remote_overlay_path;
  }
  const overlayInput = document.getElementById('remoteOverlayPathInput');
  const overlayRoot = overlayInput ? overlayInput.value.trim().replace(/\/+$/, '') : '';
  const overlayName = entry && entry.overlay_video_name ? entry.overlay_video_name : '';
  if (overlayRoot && overlayName) {
    return overlayRoot + '/' + overlayName;
  }
  return overlayRoot;
}

async function attachOverlay(savedId, entry) {
  const overlayPath = window.prompt(
    entry && entry.has_overlay
      ? 'Enter the replacement overlay file or directory path for this saved log.'
      : 'Enter the overlay file or directory path for this saved log.',
    suggestOverlayPath(entry) || ''
  );
  if (overlayPath === null) {
    return;
  }

  const trimmedPath = overlayPath.trim();
  if (!trimmedPath) {
    showNotification('Overlay path is required.', 'error');
    return;
  }

  try {
    const resp = await fetch(apiPath('api/saved-logs/' + savedId + '/overlay'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ overlay_path: trimmedPath })
    });
    const data = await resp.json();
    if (!data.success) {
      throw new Error(data.error || 'Overlay attach failed');
    }
    await loadMappings();
    if (mappings[String(savedId)]) {
      await updateScenario(String(savedId));
    }
    showNotification('Overlay attached.', 'success');
  } catch (error) {
    showNotification('Overlay attach failed: ' + error.message, 'error');
  }
}

async function updateScenario(key) {
  currentKey = key && mappings[key] ? key : null;
  renderSavedList();

  const logNameDisplay = document.getElementById('logNameDisplay');
  const toolbarMeta = document.getElementById('toolbarMeta');
  const video = document.getElementById('videoPlayer');
  const overlayVideo = document.getElementById('overlayVideoPlayer');
  const videoInfo = document.getElementById('videoInfo');
  const overlayVideoInfo = document.getElementById('overlayVideoInfo');
  const commentBox = document.getElementById('commentBox');
  const sensorGrid = document.getElementById('sensorGrid');

  if (!currentKey) {
    resetVideoPlayer(video, 'videoInfo', 'No video', 'primary');
    resetVideoPlayer(overlayVideo, 'overlayVideoInfo', 'No overlay', 'overlay');
    setOverlayPanelVisible(false);
    logNameDisplay.textContent = 'No stream selected';
    toolbarMeta.textContent = 'Scan matching HTML and video paths, then add the logs you want to keep.';
    commentBox.value = '';
    document.getElementById('commentPath').textContent = '';
    sensorGrid.innerHTML = '<div class="no-data">No images available</div>';
    updateSensorFilter(null);
    setVlmStatus(vlmEnabled ? '' : 'AI video description is disabled for this session.', vlmEnabled ? '' : 'error');
    setVlmBusy(false);
    return;
  }

  const entry = mappings[currentKey];
  const label = getVisibleLabel(entry, currentKey);
  logNameDisplay.textContent = label;
  const toolbarParts = [entry.remote_html_path || entry.video_name || 'Cached stream'];
  if (entry.has_overlay) {
    toolbarParts.push(entry.remote_overlay_path ? 'Overlay attached' : 'Overlay ready');
  }
  toolbarMeta.textContent = toolbarParts.join(' • ');

  video.onloadedmetadata = function () {
    videoInfo.textContent = entry.video_name || label;
  };
  video.onerror = function () {
    videoInfo.textContent = 'Video could not be loaded';
  };
  if (!entry.video) {
    resetVideoPlayer(video, 'videoInfo', 'No video', 'primary');
  } else {
    video.currentTime = 0;
    await loadVideoIntoPlayer(video, entry, label, {
      slot: 'primary',
      urlKey: 'video',
      nameKey: 'video_name',
      infoId: 'videoInfo'
    });
  }

  const hasOverlay = Boolean(entry.overlay_video || entry.overlay_video_name || entry.remote_overlay_path);
  setOverlayPanelVisible(hasOverlay);
  overlayVideo.onloadedmetadata = function () {
    overlayVideoInfo.textContent = entry.overlay_video_name || 'Overlay video';
  };
  overlayVideo.onerror = function () {
    overlayVideoInfo.textContent = hasOverlay ? 'Overlay could not be loaded' : 'No overlay';
  };
  if (!hasOverlay) {
    resetVideoPlayer(overlayVideo, 'overlayVideoInfo', 'No overlay', 'overlay');
  } else if (!entry.overlay_video) {
    resetVideoPlayer(overlayVideo, 'overlayVideoInfo', 'Overlay attached but not cached locally yet.', 'overlay');
  } else {
    overlayVideo.currentTime = 0;
    await loadVideoIntoPlayer(overlayVideo, entry, entry.overlay_video_name || (label + ' overlay'), {
      slot: 'overlay',
      urlKey: 'overlay_video',
      nameKey: 'overlay_video_name',
      infoId: 'overlayVideoInfo'
    });
  }

  updateSensorFilter(entry);
  buildSensorGrid(entry, 'all');
  await loadExistingComment(entry);
  setVlmStatus(vlmEnabled ? '' : 'AI video description is disabled for this session.', vlmEnabled ? '' : 'error');
}

function updateSensorFilter(entry) {
  const select = document.getElementById('sensorFilter');
  select.innerHTML = '<option value="all">All</option>';
  if (!entry || !entry.images) {
    return;
  }
  Object.keys(entry.images).sort().forEach(function (sensor) {
    const option = document.createElement('option');
    option.value = sensor;
    option.textContent = sensor.charAt(0).toUpperCase() + sensor.slice(1);
    select.appendChild(option);
  });
}

function buildSensorGrid(entry, filter) {
  const grid = document.getElementById('sensorGrid');
  grid.innerHTML = '';
  currentImages = [];
  if (!entry || !entry.images) {
    grid.innerHTML = '<div class="no-data">No images available</div>';
    grid.removeAttribute('data-count');
    return;
  }

  const images = entry.images || {};
  const sensors = filter === 'all' ? Object.keys(images) : [filter];
  sensors.forEach(function (sensor) {
    if (!images[sensor]) {
      return;
    }
    Object.entries(images[sensor]).forEach(function (imageEntry) {
      const position = imageEntry[0];
      const imgPath = imageEntry[1];
      currentImages.push({ path: imgPath, sensor: sensor, position: position });
      const imageIndex = currentImages.length - 1;

      const card = document.createElement('div');
      card.className = 'sensor-card';
      card.addEventListener('click', function () {
        openImageModal(imageIndex);
      });

      const img = document.createElement('img');
      img.src = imgPath;
      img.alt = sensor + ' - ' + position;
      img.loading = 'lazy';

      const label = document.createElement('div');
      label.className = 'sensor-card-label';
      label.textContent = sensor.toUpperCase() + ' - ' + position;

      card.appendChild(img);
      card.appendChild(label);
      grid.appendChild(card);
    });
  });

  if (currentImages.length === 0) {
    grid.innerHTML = '<div class="no-data">No images for this filter</div>';
    grid.removeAttribute('data-count');
  } else {
    grid.setAttribute('data-count', String(currentImages.length));
  }
}

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
  } catch (error) {
    commentBox.value = entry.comment_content || '';
  }
  commentBox.readOnly = true;
}

async function saveComment() {
  const entry = mappings[currentKey];
  if (!entry) {
    return;
  }

  const commentBox = document.getElementById('commentBox');
  try {
    const resp = await fetch(apiPath('api/save-comment'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        path: entry.comment_path,
        content: commentBox.value
      })
    });
    const data = await resp.json();
    if (!data.success) {
      throw new Error(data.error || 'Save failed');
    }
    commentBox.readOnly = true;
    showNotification('Comment saved.', 'success');
  } catch (error) {
    showNotification('Error saving comment: ' + error.message, 'error');
  }
}

function setVlmStatus(message, tone) {
  const status = document.getElementById('vlmStatus');
  status.textContent = message || '';
  status.className = 'vlm-status' + (tone ? ' ' + tone : '');
}

function setVlmBusy(isBusy) {
  vlmInFlight = isBusy;
  const button = document.getElementById('generateVlmBtn');
  if (!button) {
    return;
  }
  button.disabled = isBusy || !vlmEnabled;
  if (!vlmEnabled) {
    button.textContent = 'AI Summary Disabled';
    return;
  }
  button.textContent = isBusy ? 'Generating...' : 'Generate Summary';
}

async function generateVlmDescription(force) {
  if (!vlmEnabled) {
    setVlmStatus('AI video description is disabled for this session.', 'error');
    return;
  }

  const entry = mappings[currentKey];
  if (!entry || !entry.video) {
    setVlmStatus('No video is available for the current log.', 'error');
    return;
  }
  if (vlmInFlight) {
    return;
  }

  setVlmBusy(true);
  setVlmStatus('Submitting VLM analysis job to cluster...', 'busy');
  try {
    const resp = await fetch(apiPath('api/vlm/process'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        video_path: entry.video,
        force: Boolean(force)
      })
    });
    const data = await resp.json();
    if (!data.success) {
      throw new Error(data.error || 'Failed to generate description');
    }

    // Fast path: .txt already existed on cluster
    if (data.status === 'skipped' || data.status === 'processed') {
      if (typeof data.description === 'string') {
        entry.comment_content = data.description;
      }
      await loadExistingComment(entry);
      setVlmStatus(
        data.status === 'skipped'
          ? 'A summary already existed for this video. The existing text was kept.'
          : 'Summary generated and saved to the log comment file.',
        'success'
      );
      setVlmBusy(false);
      return;
    }

    // Async path: job submitted to Slurm, poll for completion
    if (data.status === 'submitted' && data.job_id) {
      setVlmStatus('Job submitted to Slurm. Waiting for compute resources...', 'busy');
      const jobId = data.job_id;
      const maxPolls = 60; // 5 minutes
      for (let i = 0; i < maxPolls; i++) {
        await new Promise(r => setTimeout(r, 5000));
        try {
          const statusResp = await fetch(apiPath('api/vlm/job/' + jobId));
          const statusData = await statusResp.json();
          if (statusData.status === 'completed') {
            // Reload the comment — the .txt file should now exist locally
            await loadExistingComment(entry);
            if (entry.comment_content) {
              setVlmStatus('Summary generated successfully on the cluster.', 'success');
            } else {
              setVlmStatus('Job completed. Refresh the page to see the summary.', 'success');
            }
            setVlmBusy(false);
            return;
          }
          if (statusData.status === 'failed' || statusData.status === 'cancelled' || statusData.status === 'timeout') {
            throw new Error('Cluster job ' + statusData.status + ': ' + (statusData.error || ''));
          }
          // still running — update message periodically
          if (i === 2) setVlmStatus('Allocating 64 GB node for VLM analysis...', 'busy');
          if (i === 6) setVlmStatus('Running Gemma-4 VLM inference on the cluster...', 'busy');
        } catch (pollErr) {
          if (pollErr.message && pollErr.message.includes('Cluster job')) throw pollErr;
          // network glitch — keep polling
        }
      }
      throw new Error('Timed out waiting for cluster job. Check back later (Job ID: ' + jobId + ').');
    }

    throw new Error(data.error || 'Unexpected response from server');
  } catch (error) {
    setVlmStatus('Error generating summary: ' + error.message, 'error');
  } finally {
    setVlmBusy(false);
  }
}

function openHtmlPreview() {
  const entry = mappings[currentKey];
  if (!entry || !entry.html_files || entry.html_files.length === 0) {
    showNotification('No HTML files available.', 'error');
    return;
  }
  window.open(entry.html_files[0], '_blank');
}

function setupExpandButtons() {
  document.querySelectorAll('.expand-btn').forEach(function (button) {
    button.addEventListener('click', function (event) {
      event.stopPropagation();
      const box = button.closest('.box');
      box.classList.toggle('expanded');
      button.textContent = box.classList.contains('expanded') ? '✕' : '⛶';
    });
  });
}

function setupImageModal() {
  const modal = document.getElementById('imageModal');
  modal.querySelector('.image-modal-close').addEventListener('click', closeImageModal);
  modal.querySelector('.image-modal-nav.prev').addEventListener('click', function () {
    navigateModal(-1);
  });
  modal.querySelector('.image-modal-nav.next').addEventListener('click', function () {
    navigateModal(1);
  });
  modal.addEventListener('click', function (event) {
    if (event.target === modal) {
      closeImageModal();
    }
  });
  document.addEventListener('keydown', function (event) {
    if (event.key === 'Escape') {
      if (modal.classList.contains('active')) {
        closeImageModal();
      }
      if (document.getElementById('clusterModal').classList.contains('active')) {
        closeClusterModal();
      }
      return;
    }
    if (!modal.classList.contains('active')) {
      return;
    }
    if (event.key === 'ArrowLeft') {
      navigateModal(-1);
    } else if (event.key === 'ArrowRight') {
      navigateModal(1);
    }
  });
}

function openImageModal(index) {
  if (!currentImages[index]) {
    return;
  }
  currentImageIndex = index;
  const modal = document.getElementById('imageModal');
  const img = modal.querySelector('img');
  const info = modal.querySelector('.image-modal-info');
  const imageData = currentImages[index];
  img.src = imageData.path;
  info.textContent = imageData.sensor.toUpperCase() + ' - ' + imageData.position + ' (' + (index + 1) + '/' + currentImages.length + ')';
  modal.classList.add('active');
}

function closeImageModal() {
  document.getElementById('imageModal').classList.remove('active');
}

function navigateModal(direction) {
  if (!currentImages.length) {
    return;
  }
  currentImageIndex = (currentImageIndex + direction + currentImages.length) % currentImages.length;
  openImageModal(currentImageIndex);
}

async function openClusterModal() {
  document.getElementById('clusterModal').classList.add('active');
  const modalContent = document.querySelector('#clusterModal .modal-content');
  if (modalContent) {
    modalContent.scrollTop = 0;
  }
  setConnectMode(connectMode);
  await checkClusterStatus(true);
}

function closeClusterModal() {
  hideClusterAuth(true);
  document.getElementById('clusterModal').classList.remove('active');
}

function applyClusterStatus(data) {
  if (!data || typeof data !== 'object') {
    return;
  }

  clusterStatusState = data;
  const htmlEl = document.getElementById('remoteHtmlPathInput');
  const videoEl = document.getElementById('remoteVideoPathInput');
  const overlayEl = document.getElementById('remoteOverlayPathInput');
  const outputEl = document.getElementById('outputPathInput');
  const netIdEl = document.getElementById('netIdInput');
  const authNetIdDisplay = document.getElementById('authNetIdDisplay');
  if (htmlEl && !htmlEl.value && data.html_path) {
    htmlEl.value = data.html_path;
  }
  if (videoEl && !videoEl.value && data.video_path) {
    videoEl.value = data.video_path;
  }
  if (overlayEl && !overlayEl.value && data.overlay_path) {
    overlayEl.value = data.overlay_path;
  }
  if (outputEl) {
    outputEl.value = data.output_root || '';
  }
  if (netIdEl && data.net_id) {
    netIdEl.value = data.net_id;
  }
  if (authNetIdDisplay) {
    let label = data.net_id ? 'Net ID: ' + data.net_id : '';
    if (label && data.has_saved_password) {
      label += ' - saved access ready';
    }
    authNetIdDisplay.textContent = label;
  }
  updateClusterUI(Boolean(data.connected), data.server || null);
}

async function checkClusterStatus(forceRefresh) {
  if (clusterStatusRequest) {
    return clusterStatusRequest;
  }
  if (!forceRefresh && clusterStatusState) {
    applyClusterStatus(clusterStatusState);
    return clusterStatusState;
  }

  clusterStatusRequest = (async function () {
    try {
      const resp = await fetch(apiPath('api/cluster/status'), { credentials: 'same-origin' });
      const data = await resp.json();
      applyClusterStatus(data);
      return data;
    } catch (error) {
      console.error('Failed to check cluster status:', error);
      return clusterStatusState || {};
    } finally {
      clusterStatusRequest = null;
    }
  })();
  return clusterStatusRequest;
}

async function resolveClusterAuthContext(forceRefresh) {
  const status = await checkClusterStatus(forceRefresh);
  const netIdEl = document.getElementById('netIdInput');
  const passwordEl = document.getElementById('passwordInput');
  const netId = ((netIdEl && netIdEl.value) || (status && status.net_id) || '').trim();
  if (netIdEl && netId && !netIdEl.value) {
    netIdEl.value = netId;
  }
  return {
    status: status || {},
    netId: netId,
    password: passwordEl ? passwordEl.value : '',
    hasSavedPassword: Boolean(status && status.has_saved_password && status.net_id && status.net_id.trim() === netId),
  };
}

function updateClusterUI(connected, serverName) {
  const statusDot = document.getElementById('statusDot');
  const statusText = document.getElementById('statusText');
  const clusterPanel = document.getElementById('clusterPanel');
  const localPanel = document.getElementById('localPanel');
  const authSheet = document.getElementById('clusterCredentialsRow');

  statusDot.className = 'status-dot ' + (connected ? 'connected' : 'disconnected');
  if (connected) {
    statusText.textContent = 'Streaming from ' + serverName;
  } else if (clusterStatusState && clusterStatusState.has_saved_password) {
    statusText.textContent = 'Ready to scan with saved access';
  } else {
    statusText.textContent = 'Ready to scan';
  }

  if (connectMode === 'local') {
    clusterPanel.classList.remove('active');
    localPanel.classList.add('active');
    return;
  }

  localPanel.classList.remove('active');
  clusterPanel.classList.add('active');
  if (authSheet && (connected || (clusterStatusState && clusterStatusState.has_saved_password))) {
    authSheet.style.display = 'none';
  }
}

function setConnectMode(mode) {
  connectMode = mode === 'local' ? 'local' : 'cluster';
  document.getElementById('modeStreamBtn').classList.toggle('active', connectMode === 'cluster');
  document.getElementById('modeLocalBtn').classList.toggle('active', connectMode === 'local');
  updateClusterUI(Boolean(clusterStatusState && clusterStatusState.connected), clusterStatusState ? clusterStatusState.server || null : null);
}

function togglePassword() {
  const input = document.getElementById('passwordInput');
  const button = document.getElementById('togglePasswordBtn');
  if (input.type === 'password') {
    input.type = 'text';
    button.textContent = '🙈';
  } else {
    input.type = 'password';
    button.textContent = '👁';
  }
}

function showClusterAuth(actionName) {
  pendingClusterAction = actionName || null;
  const authSheet = document.getElementById('clusterCredentialsRow');
  authSheet.style.display = 'block';
  document.getElementById('passwordInput').focus();
}

function hideClusterAuth(resetPending) {
  if (resetPending) {
    pendingClusterAction = null;
  }
  document.getElementById('clusterCredentialsRow').style.display = 'none';
}

async function clusterConnect() {
  const htmlPath = document.getElementById('remoteHtmlPathInput').value.trim();
  const videoPath = document.getElementById('remoteVideoPathInput').value.trim();
  const overlayPath = document.getElementById('remoteOverlayPathInput').value.trim();
  const outputPath = document.getElementById('outputPathInput').value.trim();
  const authContext = await resolveClusterAuthContext(true);
  const netId = authContext.netId;
  const password = authContext.password;
  const usingSavedPassword = authContext.hasSavedPassword && !password;

  if (!htmlPath || !videoPath) {
    showClusterMessage('Enter both HTML and video paths first.', 'error');
    return false;
  }
  if (!netId) {
    showClusterMessage('No Net ID is available for this session. Open Hyperlink from your logged-in dashboard session and try again.', 'error');
    return false;
  }
  if (!password && !authContext.hasSavedPassword) {
    showClusterAuth(pendingClusterAction);
    showClusterMessage('Enter your cluster password to continue.', 'info');
    return false;
  }

  showLoading(usingSavedPassword ? 'Opening stream session with saved access...' : 'Opening stream session...');
  try {
    const resp = await fetch(apiPath('api/cluster/connect'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        html_path: htmlPath,
        video_path: videoPath,
        overlay_path: overlayPath,
        output_path: outputPath,
        net_id: netId,
        password: password,
      })
    });
    const data = await resp.json();
    hideLoading();

    if (!data.success) {
      throw new Error(data.error || 'Connection failed');
    }

    document.getElementById('passwordInput').value = '';
    hideClusterAuth(false);
    clusterStatusState = null;
    const status = await checkClusterStatus(true);
    updateClusterUI(true, data.server || data.cluster || (status && status.server) || 'cluster');
    showClusterMessage(usingSavedPassword ? 'Stream session ready using saved access.' : 'Stream session ready.', 'success');

    const action = pendingClusterAction;
    pendingClusterAction = null;
    if (action === 'scan') {
      await scanRemoteLogs(true);
    }
    return true;
  } catch (error) {
    hideLoading();
    showClusterMessage('Connection error: ' + error.message, 'error');
    return false;
  }
}

async function ensureClusterConnected(actionName) {
  const authContext = await resolveClusterAuthContext(true);
  if (authContext.status && authContext.status.connected) {
    updateClusterUI(true, authContext.status.server || 'cluster');
    return true;
  }

  if (!authContext.netId) {
    showClusterMessage('No Net ID is available for this session. Open Hyperlink from your logged-in dashboard session and try again.', 'error');
    return false;
  }

  if (!authContext.password && !authContext.hasSavedPassword) {
    showClusterAuth(actionName);
    showClusterMessage('Enter your cluster password to scan remote logs.', 'info');
    return false;
  }
  return clusterConnect();
}

async function clusterDisconnect() {
  try {
    await fetch(apiPath('api/cluster/disconnect'), { method: 'POST' });
    remoteLogs = {};
    document.getElementById('remoteLogsList').innerHTML = '';
    document.getElementById('remoteLogsSection').style.display = 'none';
    document.getElementById('passwordInput').value = '';
    hideClusterAuth(true);
    clusterStatusState = null;
    await checkClusterStatus(true);
    showClusterMessage('Session reset.', 'info');
  } catch (error) {
    console.error('Disconnect error:', error);
  }
}

async function scanRemoteLogs(skipConnect) {
  const htmlPath = document.getElementById('remoteHtmlPathInput').value.trim();
  const videoPath = document.getElementById('remoteVideoPathInput').value.trim();
  const overlayPath = document.getElementById('remoteOverlayPathInput').value.trim();
  const outputPath = document.getElementById('outputPathInput').value.trim();

  if (!htmlPath || !videoPath) {
    showClusterMessage('Enter both HTML and video paths.', 'error');
    return;
  }

  if (!skipConnect) {
    const connected = await ensureClusterConnected('scan');
    if (!connected) {
      return;
    }
  }

  showLoading('Scanning matching pairs...');
  try {
    const resp = await fetch(apiPath('api/cluster/scan-logs'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        html_path: htmlPath,
        video_path: videoPath,
        overlay_path: overlayPath,
        output_path: outputPath,
      })
    });
    const data = await resp.json();
    hideLoading();
    if (!data.success) {
      throw new Error(data.error || 'Scan failed');
    }
    remoteLogs = data.logs || {};
    displayRemoteLogs();
    showClusterMessage('Found ' + Object.keys(remoteLogs).length + ' matching pairs.', 'success');
  } catch (error) {
    hideLoading();
    showClusterMessage('Scan error: ' + error.message, 'error');
  }
}

function displayRemoteLogs() {
  const section = document.getElementById('remoteLogsSection');
  const list = document.getElementById('remoteLogsList');
  list.innerHTML = '';

  const logKeys = Object.keys(remoteLogs).sort(function (left, right) {
    const leftName = (remoteLogs[left] && remoteLogs[left].name) || left;
    const rightName = (remoteLogs[right] && remoteLogs[right].name) || right;
    return leftName.localeCompare(rightName);
  });

  if (!logKeys.length) {
    section.style.display = 'none';
    return;
  }

  section.style.display = 'block';
  logKeys.forEach(function (key) {
    const log = remoteLogs[key];
    const item = document.createElement('div');
    item.className = 'remote-log-item' + (log.saved ? ' cached' : '');
    item.id = 'log-' + key;

    const body = document.createElement('div');
    body.className = 'remote-log-body';

    const title = document.createElement('div');
    title.className = 'remote-log-title';
    title.textContent = log.name || key;
    title.title = title.textContent;

    const path = document.createElement('div');
    path.className = 'remote-log-path';
    path.textContent = log.video_name || log.video_path || '';
    path.title = path.textContent;

    const overlay = document.createElement('div');
    overlay.className = 'remote-log-overlay';
    overlay.textContent = log.has_overlay
      ? 'Overlay: ' + (log.overlay_name || log.overlay_path || 'matched')
      : 'No overlay matched';
    overlay.title = overlay.textContent;

    const status = document.createElement('div');
    status.className = 'remote-log-status';
    status.textContent = log.saved
      ? 'Already saved to your stream list'
      : (log.has_overlay ? 'Ready to add with overlay' : 'Ready to add');

    body.appendChild(title);
    body.appendChild(path);
    body.appendChild(overlay);
    body.appendChild(status);

    const actions = document.createElement('div');
    actions.className = 'remote-log-actions';

    const actionButton = document.createElement('button');
    actionButton.type = 'button';
    actionButton.className = 'remote-log-action' + (log.saved ? ' open' : ' add');
    actionButton.textContent = log.saved ? 'Open' : 'Add';
    actionButton.addEventListener('click', async function () {
      await downloadLog(key);
    });

    actions.appendChild(actionButton);
    item.appendChild(body);
    item.appendChild(actions);
    list.appendChild(item);
  });
}

async function downloadLog(key) {
  const log = remoteLogs[key];
  if (!log) {
    return;
  }

  if (log.saved && log.saved_id) {
    await loadMappings();
    if (mappings[String(log.saved_id)]) {
      await updateScenario(String(log.saved_id));
      closeClusterModal();
    }
    return;
  }

  const item = document.getElementById('log-' + key);
  const actionButton = item ? item.querySelector('.remote-log-action') : null;
  if (actionButton) {
    actionButton.disabled = true;
    actionButton.textContent = 'Adding...';
  }

  showLoading('Adding ' + (log.name || key) + '...');
  try {
    const resp = await fetch(apiPath('api/cluster/download-log'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        key: key,
        html_path: log.html_path,
        video_path: log.video_path,
        overlay_path: log.overlay_path || '',
        output_path: document.getElementById('outputPathInput').value.trim(),
      })
    });
    const data = await resp.json();
    hideLoading();
    if (!data.success) {
      throw new Error(data.error || 'Add failed');
    }
    log.saved = true;
    log.cached = true;
    log.saved_id = data.saved_id;
    await loadMappings();
    displayRemoteLogs();
    if (data.saved_id && mappings[String(data.saved_id)]) {
      await updateScenario(String(data.saved_id));
    }
    showClusterMessage('Added to your stream list.', 'success');
  } catch (error) {
    hideLoading();
    if (actionButton) {
      actionButton.disabled = false;
      actionButton.textContent = 'Add';
    }
    showClusterMessage('Add error: ' + error.message, 'error');
  }
}

async function localUpload() {
  const htmlInput = document.getElementById('localHtmlFolderInput');
  const videoInput = document.getElementById('localVideoFilesInput');
  const outputPath = document.getElementById('localOutputPathInput').value.trim();
  const htmlFiles = Array.from(htmlInput.files || []);
  const videoFiles = Array.from(videoInput.files || []);

  if (!htmlFiles.length && !videoFiles.length) {
    showClusterMessage('Please select HTML files or videos to upload.', 'error');
    return;
  }

  const formData = new FormData();
  formData.append('output_path', outputPath);
  htmlFiles.forEach(function (file) {
    const rel = file.webkitRelativePath || file.name;
    formData.append('html_files', file, rel);
  });
  videoFiles.forEach(function (file) {
    formData.append('video_files', file, file.name);
  });

  showLoading('Uploading files to the server...');
  try {
    const resp = await fetch(apiPath('api/local/upload'), { method: 'POST', body: formData });
    const data = await resp.json();
    hideLoading();
    if (!data.success) {
      throw new Error(data.error || 'Upload failed');
    }
    closeClusterModal();
    await loadMappings();
    showNotification('Upload complete.', 'success');
  } catch (error) {
    hideLoading();
    showClusterMessage('Upload error: ' + error.message, 'error');
  }
}

function showLoading(text) {
  document.getElementById('loadingText').textContent = text || 'Loading...';
  document.getElementById('loadingOverlay').style.display = 'flex';
}

function hideLoading() {
  document.getElementById('loadingOverlay').style.display = 'none';
}

function showClusterMessage(message, type) {
  const el = document.getElementById('clusterMessage');
  el.textContent = message;
  el.className = 'cluster-message ' + type;
  el.style.display = 'block';
  window.clearTimeout(el._timer);
  el._timer = window.setTimeout(function () {
    el.style.display = 'none';
  }, 4000);
}

function showNotification(message, type) {
  let toast = document.getElementById('viewerToast');
  if (!toast) {
    toast = document.createElement('div');
    toast.id = 'viewerToast';
    document.body.appendChild(toast);
  }
  toast.className = 'viewer-toast ' + (type || 'info');
  toast.textContent = message;
  toast.classList.add('visible');
  window.clearTimeout(toastTimer);
  toastTimer = window.setTimeout(function () {
    toast.classList.remove('visible');
  }, 2600);
}
