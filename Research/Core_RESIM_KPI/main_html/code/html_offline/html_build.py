"""
HTML Builder for Offline Mode
Generates a standalone HTML file with embedded CSS and JavaScript.
UI Layout: 1:3 ratio (left:right), images on right side, HTML button at bottom.
"""
import json
from typing import Any, Dict
import os


def build_html(out_path: str, mapping: Dict[str, Dict[str, Any]], serve_mode: bool = False) -> None:
    """Build standalone HTML file for offline viewing"""
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    data_json = json.dumps(mapping, indent=2)
    
    html_template = """<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Log Viewer - Offline</title>
  <style>
    * { box-sizing: border-box; }
    
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
      margin: 0;
      padding: 0;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      overflow: hidden;
    }
    
    .page {
      padding: 10px;
      height: 100vh;
      display: flex;
      flex-direction: column;
    }
    
    /* Toolbar */
    .toolbar {
      display: flex;
      align-items: center;
      gap: 12px;
      margin-bottom: 10px;
      padding: 10px 16px;
      background: rgba(255, 255, 255, 0.95);
      backdrop-filter: blur(10px);
      border-radius: 8px;
      box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
    }
    
    .toolbar label {
      font-weight: 600;
      color: #2d3748;
      font-size: 13px;
    }
    
    .toolbar select {
      padding: 6px 10px;
      border: 1px solid #e2e8f0;
      border-radius: 6px;
      background: white;
      font-size: 13px;
      cursor: pointer;
      outline: none;
    }
    
    .toolbar select:focus {
      border-color: #667eea;
      box-shadow: 0 0 0 2px rgba(102, 126, 234, 0.2);
    }
    
    .log-name-display {
      font-weight: 600;
      color: #4a5568;
      margin-left: 8px;
    }
    
    .toolbar-spacer { flex: 1; }
    
    /* Main Layout - 1:3 ratio */
    .layout {
      display: grid;
      grid-template-columns: 1fr 3fr;
      gap: 10px;
      flex: 1;
      min-height: 0;
    }
    
    /* Left Column */
    .left-column {
      display: flex;
      flex-direction: column;
      gap: 10px;
      min-height: 0;
    }
    
    /* Box Base */
    .box {
      border-radius: 8px;
      padding: 10px;
      background: rgba(255, 255, 255, 0.98);
      overflow: hidden;
      box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
      position: relative;
      display: flex;
      flex-direction: column;
    }
    
    /* Video Box */
    .video-box {
      flex: 1;
      min-height: 150px;
    }
    
    .video-box video {
      width: 100%;
      flex: 1;
      min-height: 0;
      background: #000;
      border-radius: 4px;
      object-fit: contain;
    }
    
    .video-info {
      font-size: 11px;
      color: #4a5568;
      padding: 4px 6px;
      background: #f7f7f7;
      border-radius: 4px;
      margin-top: 4px;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
    }
    
    /* Text Box */
    .text-box {
      flex: 1;
      min-height: 120px;
    }
    
    #commentBox {
      flex: 1;
      width: 100%;
      resize: none;
      padding: 8px;
      font-family: 'Consolas', monospace;
      font-size: 11px;
      border: 1px solid #e2e8f0;
      border-radius: 6px;
      outline: none;
      background: #f8fafc;
      line-height: 1.4;
    }
    
    #commentBox:focus {
      border-color: #667eea;
      background: white;
    }
    
    .text-path {
      font-size: 9px;
      color: #718096;
      margin-top: 4px;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
    }
    
    /* Right Column - Images */
    .right-column {
      display: flex;
      flex-direction: column;
      min-height: 0;
    }
    
    .image-box {
      flex: 1;
      display: flex;
      flex-direction: column;
    }
    
    .image-header {
      display: flex;
      align-items: center;
      gap: 10px;
      margin-bottom: 8px;
      padding-bottom: 6px;
      border-bottom: 1px solid #eee;
    }
    
    .image-header label {
      font-weight: 600;
      color: #2d3748;
      font-size: 12px;
    }
    
    .image-header select {
      padding: 4px 8px;
      border: 1px solid #e2e8f0;
      border-radius: 4px;
      background: white;
      font-size: 11px;
      cursor: pointer;
      outline: none;
    }
    
    /* Sensor Grid - Adaptive */
    .sensor-grid {
      display: grid;
      grid-template-columns: repeat(auto-fill, minmax(180px, 1fr));
      gap: 8px;
      overflow-y: auto;
      flex: 1;
      padding: 4px;
      align-content: start;
    }
    
    .sensor-grid::-webkit-scrollbar {
      width: 5px;
    }
    
    .sensor-grid::-webkit-scrollbar-thumb {
      background: #ccc;
      border-radius: 3px;
    }
    
    .sensor-card {
      background: white;
      border: 1px solid #e8e8e8;
      border-radius: 6px;
      padding: 4px;
      display: flex;
      flex-direction: column;
      align-items: center;
      cursor: pointer;
      transition: all 0.15s ease;
      aspect-ratio: 4/3;
    }
    
    .sensor-card:hover {
      border-color: #667eea;
      box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
    }
    
    .sensor-card img {
      width: 100%;
      height: 100%;
      object-fit: contain;
    }
    
    .sensor-card-label {
      font-size: 9px;
      color: #666;
      margin-top: 2px;
      text-align: center;
    }
    
    /* Open HTML Button - Fixed bottom right */
    .open-html-btn {
      position: fixed;
      bottom: 16px;
      right: 16px;
      padding: 10px 18px;
      background: linear-gradient(135deg, #ff69b4, #ff1493);
      color: white;
      border: none;
      border-radius: 8px;
      font-weight: 600;
      font-size: 12px;
      cursor: pointer;
      display: flex;
      align-items: center;
      gap: 6px;
      box-shadow: 0 4px 16px rgba(255, 105, 180, 0.4);
      transition: all 0.3s ease;
      z-index: 1000;
    }
    
    .open-html-btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 6px 20px rgba(255, 105, 180, 0.5);
    }
    
    /* Expand Button */
    .expand-btn {
      position: absolute;
      top: 4px;
      right: 4px;
      width: 22px;
      height: 22px;
      border-radius: 4px;
      border: 1px solid #667eea;
      background: white;
      color: #667eea;
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 11px;
      font-weight: bold;
      transition: all 0.15s ease;
      z-index: 100;
      opacity: 0.6;
    }
    
    .box:hover .expand-btn {
      opacity: 1;
    }
    
    .expand-btn:hover {
      background: #667eea;
      color: white;
    }
    
    .expanded {
      position: fixed !important;
      top: 10px !important;
      left: 10px !important;
      right: 10px !important;
      bottom: 10px !important;
      z-index: 9999 !important;
      width: calc(100vw - 20px) !important;
      height: calc(100vh - 20px) !important;
      background: white !important;
    }
    
    .expanded .expand-btn {
      opacity: 1 !important;
      background: #ff6b6b;
      color: white;
      border-color: #ff6b6b;
    }
    
    .expanded .sensor-grid {
      grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
    }
    
    /* Image Modal */
    .image-modal {
      display: none;
      position: fixed;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background: rgba(0, 0, 0, 0.9);
      z-index: 10000;
      align-items: center;
      justify-content: center;
    }
    
    .image-modal.active {
      display: flex;
    }
    
    .image-modal img {
      max-width: 95%;
      max-height: 95%;
      object-fit: contain;
    }
    
    .image-modal-close {
      position: absolute;
      top: 20px;
      right: 30px;
      font-size: 40px;
      color: white;
      cursor: pointer;
    }
    
    .image-modal-nav {
      position: absolute;
      top: 50%;
      transform: translateY(-50%);
      font-size: 50px;
      color: white;
      cursor: pointer;
      user-select: none;
      padding: 20px;
    }
    
    .image-modal-nav.prev { left: 20px; }
    .image-modal-nav.next { right: 20px; }
    
    .image-modal-info {
      position: absolute;
      bottom: 20px;
      left: 50%;
      transform: translateX(-50%);
      color: white;
      font-size: 13px;
      background: rgba(0, 0, 0, 0.5);
      padding: 6px 14px;
      border-radius: 4px;
    }
    
    .no-data {
      color: #999;
      font-style: italic;
      font-size: 12px;
    }
    
    /* Responsive */
    @media (max-width: 1000px) {
      .layout {
        grid-template-columns: 1fr 2fr;
      }
      .sensor-grid {
        grid-template-columns: repeat(auto-fill, minmax(140px, 1fr));
      }
    }
    
    @media (max-width: 700px) {
      .layout {
        grid-template-columns: 1fr;
        grid-template-rows: auto 1fr;
      }
      .left-column {
        flex-direction: row;
      }
      .video-box, .text-box {
        flex: 1;
        min-height: 120px;
      }
    }
  </style>
</head>
<body>
  <div class="page">
    <!-- Toolbar -->
    <div class="toolbar">
      <label for="logSelect">Log name</label>
      <select id="logSelect"></select>
      <span id="logNameDisplay" class="log-name-display"></span>
      <div class="toolbar-spacer"></div>
    </div>

    <!-- Main Layout -->
    <div class="layout">
      <!-- Left Column: Video + Text -->
      <div class="left-column">
        <!-- Video Box -->
        <div class="box video-box">
          <button class="expand-btn" onclick="toggleExpand(this)">⛶</button>
          <video id="videoPlayer" controls></video>
          <div id="videoInfo" class="video-info no-data">No video</div>
        </div>

        <!-- Text Box -->
        <div class="box text-box">
          <button class="expand-btn" onclick="toggleExpand(this)">⛶</button>
          <textarea id="commentBox" readonly placeholder="No comments available"></textarea>
          <div id="commentPath" class="text-path"></div>
        </div>
      </div>

      <!-- Right Column: Images -->
      <div class="right-column">
        <div class="box image-box">
          <button class="expand-btn" onclick="toggleExpand(this)">⛶</button>
          <div class="image-header">
            <label>Sensor:</label>
            <select id="sensorFilter" onchange="filterSensors()">
              <option value="all">All</option>
            </select>
          </div>
          <div id="sensorGrid" class="sensor-grid"></div>
        </div>
      </div>
    </div>

    <!-- Open HTML Button -->
    <button id="openHtmlBtn" class="open-html-btn" onclick="openHtmlPreview()">
      📄 Open HTML Report
    </button>
  </div>

  <!-- Image Modal -->
  <div id="imageModal" class="image-modal" onclick="if(event.target===this)closeImageModal()">
    <span class="image-modal-close" onclick="closeImageModal()">&times;</span>
    <span class="image-modal-nav prev" onclick="navigateModal(-1)">❮</span>
    <img id="modalImage" src="" alt="Full size image">
    <span class="image-modal-nav next" onclick="navigateModal(1)">❯</span>
    <div id="modalInfo" class="image-modal-info"></div>
  </div>

  <script>
    // Data
    const mappings = """ + data_json + """;
    let currentKey = null;
    let currentImages = [];
    let currentImageIndex = 0;

    // Initialize
    document.addEventListener('DOMContentLoaded', function() {
      populateLogDropdown();
      setupKeyboardNav();
    });

    function populateLogDropdown() {
      const select = document.getElementById('logSelect');
      const keys = Object.keys(mappings).sort();
      
      if (keys.length === 0) {
        select.innerHTML = '<option value="">No data available</option>';
        return;
      }
      
      keys.forEach(key => {
        const opt = document.createElement('option');
        opt.value = key;
        opt.textContent = mappings[key].html_folder || key;
        select.appendChild(opt);
      });
      
      select.addEventListener('change', function() {
        updateScenario(this.value);
      });
      
      // Select first
      if (keys.length > 0) {
        updateScenario(keys[0]);
      }
    }

    function updateScenario(key) {
      currentKey = key;
      const entry = mappings[key];
      
      // Update log name display
      document.getElementById('logNameDisplay').textContent = entry ? entry.html_folder || key : '';
      
      // Update video
      const videoPlayer = document.getElementById('videoPlayer');
      const videoInfo = document.getElementById('videoInfo');
      
      if (entry && entry.video) {
        videoPlayer.src = entry.video;
        videoPlayer.load();
        videoInfo.textContent = entry.video_name || entry.video;
        videoInfo.classList.remove('no-data');
      } else {
        videoPlayer.removeAttribute('src');
        videoInfo.textContent = 'No video available';
        videoInfo.classList.add('no-data');
      }
      
      // Update comment
      const commentBox = document.getElementById('commentBox');
      const commentPath = document.getElementById('commentPath');
      commentBox.value = entry && entry.comment_content ? entry.comment_content : '';
      commentPath.textContent = entry ? entry.comment_path : '';
      
      // Update sensor dropdown and grid
      populateSensorDropdown(entry);
      buildSensorGrid(entry, 'all');
    }

    function populateSensorDropdown(entry) {
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

    function filterSensors() {
      const filter = document.getElementById('sensorFilter').value;
      const entry = mappings[currentKey];
      buildSensorGrid(entry, filter);
    }

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
      }
    }

    function toggleExpand(btn) {
      const box = btn.closest('.box');
      box.classList.toggle('expanded');
      btn.textContent = box.classList.contains('expanded') ? '✕' : '⛶';
    }

    function openHtmlPreview() {
      const entry = mappings[currentKey];
      if (!entry || !entry.html_files || entry.html_files.length === 0) {
        alert('No HTML files available');
        return;
      }
      window.open(entry.html_files[0], '_blank');
    }

    // Image Modal
    function openImageModal(index) {
      currentImageIndex = index;
      const modal = document.getElementById('imageModal');
      const img = document.getElementById('modalImage');
      const info = document.getElementById('modalInfo');
      
      const imageData = currentImages[index];
      img.src = imageData.path;
      info.textContent = imageData.sensor.toUpperCase() + ' - ' + imageData.position + ' (' + (index + 1) + '/' + currentImages.length + ')';
      
      modal.classList.add('active');
    }

    function closeImageModal() {
      document.getElementById('imageModal').classList.remove('active');
    }

    function navigateModal(direction) {
      currentImageIndex = (currentImageIndex + direction + currentImages.length) % currentImages.length;
      const img = document.getElementById('modalImage');
      const info = document.getElementById('modalInfo');
      
      const imageData = currentImages[currentImageIndex];
      img.src = imageData.path;
      info.textContent = imageData.sensor.toUpperCase() + ' - ' + imageData.position + ' (' + (currentImageIndex + 1) + '/' + currentImages.length + ')';
    }

    function setupKeyboardNav() {
      document.addEventListener('keydown', function(e) {
        const modal = document.getElementById('imageModal');
        if (!modal.classList.contains('active')) return;
        
        if (e.key === 'Escape') closeImageModal();
        else if (e.key === 'ArrowLeft') navigateModal(-1);
        else if (e.key === 'ArrowRight') navigateModal(1);
      });
    }
  </script>
</body>
</html>
"""
    
    with open(out_path, 'w', encoding='utf-8') as f:
        f.write(html_template)
