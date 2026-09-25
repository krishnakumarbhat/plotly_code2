import json
from typing import Any, Dict
import os

def build_html(out_path: str, mapping: Dict[str, Dict[str, Any]], serve_mode: bool = False) -> None:
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    data_json = json.dumps(mapping, indent=2)
    # keep serve_mode parameter for compatibility, but button is now always visible
    serve_mode_js = "true" if serve_mode else "false"
    html_template = """<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>Log Viewer</title>
  <style>
    * {
      box-sizing: border-box;
    }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
      margin: 0;
      padding: 0;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      overflow: hidden;
    }
    .page {
      padding: 16px;
      height: 100vh;
      display: flex;
      flex-direction: column;
    }
    .toolbar {
      display: flex;
      align-items: center;
      gap: 12px;
      margin-bottom: 16px;
      padding: 16px 20px;
      background: rgba(255, 255, 255, 0.95);
      backdrop-filter: blur(10px);
      border-radius: 12px;
      box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
      animation: slideDown 0.5s ease-out;
    }
    @keyframes slideDown {
      from {
        opacity: 0;
        transform: translateY(-10px);
      }
      to {
        opacity: 1;
        transform: translateY(0);
      }
    }
    .toolbar label {
      font-weight: 600;
      color: #2d3748;
      font-size: 14px;
    }
    .toolbar select {
      padding: 8px 12px;
      border: 2px solid #e2e8f0;
      border-radius: 8px;
      background: white;
      font-size: 14px;
      cursor: pointer;
      transition: all 0.3s ease;
      outline: none;
    }
    .toolbar select:hover {
      border-color: #667eea;
      box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
    }
    .toolbar select:focus {
      border-color: #667eea;
      box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.2);
    }
    .layout {
      display: grid;
      grid-template-columns: 1fr 1fr;
      grid-template-rows: 1fr 1fr;
      gap: 16px;
      flex: 1;
      min-height: 0;
      animation: fadeIn 0.6s ease-out 0.2s both;
    }
    @keyframes fadeIn {
      from {
        opacity: 0;
      }
      to {
        opacity: 1;
      }
    }
    .box {
      border-radius: 12px;
      padding: 20px;
      background: rgba(255, 255, 255, 0.98);
      overflow: hidden;
      box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
      transition: box-shadow 0.2s ease;
      position: relative;
      display: flex;
      flex-direction: column;
      will-change: auto;
    }
    .box:hover {
      box-shadow: 0 8px 24px rgba(0, 0, 0, 0.15);
    }
    .box::before {
      content: '';
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      height: 3px;
      background: linear-gradient(90deg, #667eea, #764ba2);
      opacity: 0;
      transition: opacity 0.2s ease;
    }
    .box:hover::before {
      opacity: 1;
    }
    .resize-handle {
      position: absolute;
      background: rgba(102, 126, 234, 0.3);
      transition: background 0.3s ease;
      z-index: 10;
    }
    .resize-handle:hover {
      background: rgba(102, 126, 234, 0.6);
    }
    .resize-handle-v {
      width: 8px;
      height: 100%;
      top: 0;
      right: -4px;
      cursor: col-resize;
    }
    .resize-handle-h {
      height: 8px;
      width: 100%;
      bottom: -4px;
      left: 0;
      cursor: row-resize;
    }
    .expand-btn {
      position: absolute;
      top: 8px;
      right: 8px;
      width: 28px;
      height: 28px;
      border-radius: 6px;
      border: 1px solid #667eea;
      background: white;
      color: #667eea;
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 14px;
      font-weight: bold;
      transition: all 0.15s ease;
      z-index: 100;
      opacity: 0.8;
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
      margin: 0 !important;
      width: calc(100vw - 20px) !important;
      height: calc(100vh - 20px) !important;
      max-width: none !important;
      max-height: none !important;
      background: white !important;
    }
    .expanded .expand-btn {
      opacity: 1 !important;
      background: #ff6b6b;
      color: white;
      border-color: #ff6b6b;
    }
    .overlay {
      display: none;
    }
    .video-box {
      border-left: 2px solid #ff7f50;
    }
    .video-box video {
      width: 100%;
      flex: 1;
      min-height: 0;
      background: #000;
      border-radius: 4px;
      object-fit: contain;
    }
    .expanded.video-box video {
      height: calc(100vh - 100px);
      max-height: calc(100vh - 100px);
    }
    .video-info {
      font-size: 12px;
      color: #4a5568;
      padding: 6px 10px;
      background: #f7f7f7;
      border-radius: 4px;
      margin-top: 8px;
    }
    .image-box {
      border-left: 2px solid #1e90ff;
    }
    .image-header {
      display: flex;
      align-items: center;
      gap: 12px;
      margin-bottom: 16px;
      padding-bottom: 12px;
      border-bottom: 2px solid rgba(30, 144, 255, 0.1);
    }
    .image-header label {
      font-weight: 600;
      color: #2d3748;
      font-size: 14px;
    }
    .image-header select {
      padding: 6px 12px;
      border: 2px solid #e2e8f0;
      border-radius: 8px;
      background: white;
      font-size: 13px;
      cursor: pointer;
      transition: all 0.3s ease;
      outline: none;
    }
    .image-header select:hover {
      border-color: #1e90ff;
      box-shadow: 0 0 0 3px rgba(30, 144, 255, 0.1);
    }
    .sensor-grid {
      display: grid;
      grid-template-columns: repeat(auto-fill, minmax(180px, 1fr));
      gap: 12px;
      overflow-y: auto;
      flex: 1;
      padding: 4px;
      align-content: start;
    }
    .sensor-grid::-webkit-scrollbar {
      width: 6px;
    }
    .sensor-grid::-webkit-scrollbar-track {
      background: #f0f0f0;
      border-radius: 3px;
    }
    .sensor-grid::-webkit-scrollbar-thumb {
      background: #ccc;
      border-radius: 3px;
    }
    .expanded .sensor-grid {
      grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
    }
    .sensor-card {
      background: white;
      border: 1px solid #e0e0e0;
      border-radius: 8px;
      padding: 8px;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: flex-end;
      font-size: 12px;
      cursor: pointer;
      transition: all 0.15s ease;
      min-height: 120px;
    }
    .sensor-card:hover {
      border-color: #667eea;
      box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
    }
    .sensor-card img {
      max-width: 100%;
      max-height: 150px;
      flex: 1;
      object-fit: contain;
      margin-bottom: 6px;
    }
    .expanded .sensor-card {
      min-height: 180px;
    }
    .expanded .sensor-card img {
      max-height: 250px;
    }
    .text-box {
      border-left: 2px solid #708090;
    }
    #commentBox {
      flex: 1;
      width: 100%;
      resize: none;
      padding: 12px;
      font-family: 'Fira Code', 'Consolas', monospace;
      font-size: 13px;
      border: 2px solid #e2e8f0;
      border-radius: 8px;
      outline: none;
      transition: all 0.3s ease;
      background: #f8fafc;
      line-height: 1.6;
    }
    #commentBox:focus {
      border-color: #667eea;
      box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
      background: white;
    }
    .text-actions {
      margin-top: 12px;
      display: flex;
      justify-content: space-between;
      align-items: center;
      gap: 12px;
      animation: slideInUp 0.4s ease-out;
    }
    @keyframes slideInUp {
      from {
        opacity: 0;
        transform: translateY(20px);
      }
      to {
        opacity: 1;
        transform: translateY(0);
      }
    }
    .text-actions button {
      padding: 8px 16px;
      border-radius: 8px;
      border: none;
      cursor: pointer;
      color: #fff;
      font-weight: 600;
      font-size: 13px;
      transition: all 0.3s ease;
      box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
    }
    .text-actions button:hover {
      transform: translateY(-2px);
      box-shadow: 0 4px 12px rgba(0, 0, 0, 0.2);
    }
    .text-actions button:active {
      transform: translateY(0);
    }
    #editCommentBtn {
      background: linear-gradient(135deg, #1e90ff, #00bfff);
    }
    #editCommentBtn:hover {
      background: linear-gradient(135deg, #00bfff, #1e90ff);
    }
    #saveCommentBtn {
      background: linear-gradient(135deg, #32cd32, #00fa9a);
    }
    #saveCommentBtn:hover {
      background: linear-gradient(135deg, #00fa9a, #32cd32);
    }
    .text-path {
      font-size: 11px;
      color: #718096;
      flex: 1;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
      padding: 4px 8px;
      background: rgba(0, 0, 0, 0.03);
      border-radius: 4px;
    }
    .html-box {
      border-left: 2px solid #ff69b4;
    }
    .html-header {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 12px;
      margin-bottom: 16px;
      padding-bottom: 12px;
      border-bottom: 2px solid rgba(255, 105, 180, 0.1);
    }
    .html-header button {
      padding: 8px 16px;
      cursor: pointer;
      border: 2px solid #ff69b4;
      background: white;
      color: #ff69b4;
      border-radius: 8px;
      font-weight: 600;
      font-size: 13px;
      transition: all 0.3s ease;
    }
    .html-header button:hover:not(:disabled) {
      background: #ff69b4;
      color: white;
      transform: translateY(-2px);
      box-shadow: 0 4px 12px rgba(255, 105, 180, 0.3);
    }
    .html-header button:disabled {
      opacity: 0.4;
      cursor: not-allowed;
    }
    #htmlPreview {
      flex: 1;
      width: 100%;
      border: 2px solid #e2e8f0;
      background: #fff;
      border-radius: 8px;
      transition: border-color 0.3s ease;
    }
    #htmlPreview:hover {
      border-color: #ff69b4;
    }
    .no-data {
      color: #a0aec0;
      font-style: italic;
      font-size: 13px;
      animation: pulse 2s ease-in-out infinite;
    }
    @keyframes pulse {
      0%, 100% {
        opacity: 0.6;
      }
      50% {
        opacity: 1;
      }
    }
    .modal-overlay {
      display: none;
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: rgba(0, 0, 0, 0.9);
      z-index: 2000;
      justify-content: center;
      align-items: center;
      flex-direction: column;
      backdrop-filter: blur(8px);
      animation: modalFadeIn 0.3s ease;
    }
    @keyframes modalFadeIn {
      from {
        opacity: 0;
      }
      to {
        opacity: 1;
      }
    }
    .modal-overlay.active {
      display: flex;
    }
    .modal-content {
      position: relative;
      max-width: 90%;
      max-height: 85%;
      display: flex;
      flex-direction: column;
      align-items: center;
      animation: modalZoomIn 0.4s cubic-bezier(0.4, 0, 0.2, 1);
    }
    @keyframes modalZoomIn {
      from {
        transform: scale(0.7);
        opacity: 0;
      }
      to {
        transform: scale(1);
        opacity: 1;
      }
    }
    .modal-content img {
      max-width: 100%;
      max-height: 80vh;
      object-fit: contain;
      border-radius: 12px;
      box-shadow: 0 20px 60px rgba(0, 0, 0, 0.8);
      transition: transform 0.3s ease;
    }
    .modal-label {
      color: #fff;
      margin-top: 20px;
      font-size: 18px;
      text-align: center;
      font-weight: 600;
      padding: 12px 24px;
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      border-radius: 8px;
      animation: slideInUp 0.4s ease-out 0.2s both;
    }
    .modal-close {
      position: absolute;
      top: -50px;
      right: -10px;
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      border: 2px solid rgba(255, 255, 255, 0.3);
      border-radius: 50%;
      color: #fff;
      font-size: 28px;
      cursor: pointer;
      padding: 8px;
      width: 44px;
      height: 44px;
      display: flex;
      align-items: center;
      justify-content: center;
      transition: all 0.3s ease;
    }
    .modal-close:hover {
      background: rgba(255, 107, 107, 0.9);
      transform: rotate(90deg) scale(1.1);
      border-color: #ff6b6b;
    }
    .modal-nav {
      position: absolute;
      top: 50%;
      transform: translateY(-50%);
      background: rgba(255, 255, 255, 0.15);
      backdrop-filter: blur(10px);
      border: 2px solid rgba(255, 255, 255, 0.3);
      color: #fff;
      font-size: 32px;
      cursor: pointer;
      padding: 20px 16px;
      border-radius: 12px;
      transition: all 0.3s ease;
    }
    .modal-nav:hover {
      background: rgba(102, 126, 234, 0.9);
      transform: translateY(-50%) scale(1.1);
      border-color: #667eea;
    }
    .modal-prev {
      left: 20px;
    }
    .modal-next {
      right: 20px;
    }
    
    /* Cluster Modal Styles */
    .online-btn {
      padding: 8px 16px;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: white;
      border: none;
      border-radius: 8px;
      font-size: 14px;
      font-weight: 600;
      cursor: pointer;
      transition: all 0.3s ease;
      box-shadow: 0 2px 8px rgba(102, 126, 234, 0.3);
    }
    .online-btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 4px 12px rgba(102, 126, 234, 0.4);
    }
    .cluster-modal-overlay {
      display: none;
      position: fixed;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background: rgba(0, 0, 0, 0.5);
      z-index: 10000;
      align-items: center;
      justify-content: center;
    }
    .cluster-modal-overlay.active {
      display: flex;
    }
    .cluster-modal {
      background: white;
      border-radius: 16px;
      width: 450px;
      max-width: 90%;
      max-height: 90vh;
      overflow-y: auto;
      box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);
      animation: modalSlideIn 0.3s ease;
    }
    @keyframes modalSlideIn {
      from {
        opacity: 0;
        transform: translateY(-20px) scale(0.95);
      }
      to {
        opacity: 1;
        transform: translateY(0) scale(1);
      }
    }
    .cluster-modal-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 20px 24px;
      border-bottom: 1px solid #e2e8f0;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      border-radius: 16px 16px 0 0;
    }
    .cluster-modal-header h2 {
      margin: 0;
      color: white;
      font-size: 18px;
    }
    .cluster-close {
      background: rgba(255, 255, 255, 0.2);
      border: none;
      color: white;
      font-size: 24px;
      width: 36px;
      height: 36px;
      border-radius: 50%;
      cursor: pointer;
      transition: all 0.2s ease;
    }
    .cluster-close:hover {
      background: rgba(255, 255, 255, 0.3);
      transform: rotate(90deg);
    }
    .cluster-modal-body {
      padding: 24px;
    }
    .cluster-status {
      display: flex;
      align-items: center;
      gap: 8px;
      padding: 12px 16px;
      border-radius: 8px;
      margin-bottom: 20px;
      font-weight: 600;
    }
    .cluster-status.disconnected {
      background: #fef2f2;
      color: #dc2626;
    }
    .cluster-status.connected {
      background: #f0fdf4;
      color: #16a34a;
    }
    .cluster-status.connecting {
      background: #fefce8;
      color: #ca8a04;
    }
    .status-dot {
      width: 10px;
      height: 10px;
      border-radius: 50%;
      background: currentColor;
      animation: pulse 2s infinite;
    }
    @keyframes pulse {
      0%, 100% { opacity: 1; }
      50% { opacity: 0.5; }
    }
    .form-group {
      margin-bottom: 16px;
    }
    .form-group label {
      display: block;
      margin-bottom: 6px;
      font-weight: 600;
      color: #374151;
      font-size: 14px;
    }
    .form-group input,
    .form-group select {
      width: 100%;
      padding: 10px 14px;
      border: 2px solid #e2e8f0;
      border-radius: 8px;
      font-size: 14px;
      transition: all 0.2s ease;
    }
    .form-group input:focus,
    .form-group select:focus {
      outline: none;
      border-color: #667eea;
      box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
    }
    .cluster-btn {
      width: 100%;
      padding: 12px;
      border: none;
      border-radius: 8px;
      font-size: 14px;
      font-weight: 600;
      cursor: pointer;
      transition: all 0.2s ease;
      margin-top: 8px;
    }
    .connect-btn {
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: white;
    }
    .connect-btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 4px 12px rgba(102, 126, 234, 0.4);
    }
    .disconnect-btn {
      background: #dc2626;
      color: white;
    }
    .disconnect-btn:hover {
      background: #b91c1c;
    }
    .download-btn {
      background: linear-gradient(135deg, #10b981 0%, #059669 100%);
      color: white;
    }
    .download-btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 4px 12px rgba(16, 185, 129, 0.4);
    }
    .path-section {
      margin-top: 20px;
    }
    .path-section h3 {
      margin: 16px 0 12px 0;
      color: #374151;
      font-size: 16px;
    }
    .path-section hr {
      border: none;
      border-top: 1px solid #e2e8f0;
      margin: 0;
    }
    .cluster-message {
      margin-top: 16px;
      padding: 12px;
      border-radius: 8px;
      font-size: 14px;
      display: none;
    }
    .cluster-message.error {
      display: block;
      background: #fef2f2;
      color: #dc2626;
      border: 1px solid #fecaca;
    }
    .cluster-message.success {
      display: block;
      background: #f0fdf4;
      color: #16a34a;
      border: 1px solid #bbf7d0;
    }
    .cluster-message.info {
      display: block;
      background: #eff6ff;
      color: #2563eb;
      border: 1px solid #bfdbfe;
    }
    .progress-container {
      margin-top: 16px;
      display: none;
    }
    .progress-container.active {
      display: block;
    }
    .progress-bar-wrapper {
      background: #e2e8f0;
      border-radius: 8px;
      height: 24px;
      overflow: hidden;
      position: relative;
    }
    .progress-bar {
      background: linear-gradient(90deg, #667eea, #764ba2);
      height: 100%;
      width: 0%;
      transition: width 0.3s ease;
      border-radius: 8px;
    }
    .progress-bar.indeterminate {
      width: 100%;
      background: linear-gradient(90deg, #667eea 0%, #764ba2 25%, #667eea 50%, #764ba2 75%, #667eea 100%);
      background-size: 200% 100%;
      animation: progressIndeterminate 1.5s linear infinite;
    }
    @keyframes progressIndeterminate {
      0% { background-position: 200% 0; }
      100% { background-position: 0 0; }
    }
    .progress-text {
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 12px;
      font-weight: 600;
      color: #374151;
    }
    .progress-details {
      margin-top: 8px;
      font-size: 12px;
      color: #6b7280;
    }
    .progress-phase {
      font-weight: 600;
      color: #374151;
    }
    .progress-file {
      font-style: italic;
      white-space: nowrap;
      overflow: hidden;
      text-overflow: ellipsis;
      max-width: 100%;
    }
    .password-wrapper {
      position: relative;
      display: flex;
      align-items: center;
    }
    .password-wrapper input {
      flex: 1;
      padding-right: 45px;
    }
    .password-toggle {
      position: absolute;
      right: 8px;
      background: none;
      border: none;
      cursor: pointer;
      font-size: 18px;
      opacity: 0.6;
      transition: opacity 0.2s;
      padding: 4px 8px;
    }
    .password-toggle:hover {
      opacity: 1;
    }
    .optional-label {
      font-weight: 400;
      color: #6b7280;
      font-size: 12px;
    }
  </style>
</head>
<body>
  <div class="page">
    <div class="toolbar">
      <label for="logSelect">Log name</label>
      <select id="logSelect"></select>
      <div id="htmlInfo" class="no-data">No HTML selected.</div>
      <div style="flex:1;"></div>
      <button id="goOnlineBtn" class="online-btn" onclick="openClusterModal()">🌐 Go Online</button>
    </div>

    <div id="clusterModal" class="cluster-modal-overlay">
      <div class="cluster-modal">
        <div class="cluster-modal-header">
          <h2>Connect to Cluster</h2>
          <button class="cluster-close" onclick="closeClusterModal()">&times;</button>
        </div>
        <div class="cluster-modal-body">
          <div id="clusterStatus" class="cluster-status disconnected">
            <span class="status-dot"></span>
            <span class="status-text">Disconnected</span>
          </div>
          <div class="form-group">
            <label for="clusterServer">Server</label>
            <select id="clusterServer">
              <option value="southfield">Southfield (10.192.224.131)</option>
              <option value="krakow">Krakow (10.214.45.45)</option>
            </select>
          </div>
          <div class="form-group">
            <label for="clusterUser">Username (NetID)</label>
            <input type="text" id="clusterUser" placeholder="Enter your NetID" />
          </div>
          <div class="form-group">
            <label for="clusterPass">Password</label>
            <div class="password-wrapper">
              <input type="password" id="clusterPass" placeholder="Enter your password" />
              <button type="button" class="password-toggle" onclick="togglePassword()">👁</button>
            </div>
          </div>
          <button id="clusterConnectBtn" class="cluster-btn connect-btn" onclick="connectCluster()">Connect</button>
          <button id="clusterDisconnectBtn" class="cluster-btn disconnect-btn" style="display:none;" onclick="disconnectCluster()">Disconnect</button>
          
          <div id="pathSection" class="path-section" style="display:none;">
            <hr />
            <h3>Remote Paths</h3>
            <div class="form-group">
              <label for="htmlPathInput">HTML Path (Remote)</label>
              <input type="text" id="htmlPathInput" placeholder="/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR/html" />
            </div>
            <div class="form-group">
              <label for="videoPathInput">Video Path (Remote)</label>
              <input type="text" id="videoPathInput" placeholder="/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR/video" />
            </div>
            <div class="form-group">
              <label for="localPathInput">Local Save Path <span class="optional-label">(optional, defaults to 'db')</span></label>
              <input type="text" id="localPathInput" placeholder="db" />
            </div>
            <button class="cluster-btn download-btn" onclick="downloadFromCluster()">📥 Download & Load</button>
            <div id="progressContainer" class="progress-container">
              <div class="progress-bar-wrapper">
                <div id="progressBar" class="progress-bar"></div>
                <div id="progressText" class="progress-text">0%</div>
              </div>
              <div class="progress-details">
                <div id="progressPhase" class="progress-phase">Preparing...</div>
                <div id="progressFile" class="progress-file"></div>
              </div>
            </div>
          </div>
          <div id="clusterMessage" class="cluster-message"></div>
        </div>
      </div>
    </div>

    <div class="layout">
      <div class="box video-box" data-box="video">
        <button class="expand-btn" onclick="toggleExpand(this)">⛶</button>
        <div class="resize-handle resize-handle-v"></div>
        <div class="resize-handle resize-handle-h"></div>
        <video id="videoPlayer" controls></video>
        <div id="videoInfo" class="video-info no-data"></div>
      </div>

      <div class="box image-box" data-box="image">
        <button class="expand-btn" onclick="toggleExpand(this)">⛶</button>
        <div class="resize-handle resize-handle-h"></div>
        <div class="image-header">
          <label for="sensorSelect">Sensor:</label>
          <select id="sensorSelect">
            <option value="all">All</option>
          </select>
        </div>
        <div id="sensorGrid" class="sensor-grid"></div>
      </div>

      <div class="box text-box" data-box="text">
        <button class="expand-btn" onclick="toggleExpand(this)">⛶</button>
        <div class="resize-handle resize-handle-v"></div>
        <textarea id="commentBox" readonly></textarea>
        <div class="text-actions">
          <div id="commentPath" class="text-path no-data">No video selected.</div>
          <button id="editCommentBtn">Edit</button>
          <button id="saveCommentBtn" style="display:none;">Save</button>
        </div>
      </div>

      <div class="box html-box" data-box="html">
        <button class="expand-btn" onclick="toggleExpand(this)">⛶</button>
        <div class="html-header">
          <span id="htmlPreviewLabel" class="no-data">No HTML preview.</span>
          <button id="openHtmlBtn" disabled>Open</button>
        </div>
        <iframe id="htmlPreview"></iframe>
      </div>
    </div>
  </div>

  <div id="imageModal" class="modal-overlay">
    <div class="modal-content">
      <button class="modal-close" id="modalClose">&times;</button>
      <button class="modal-nav modal-prev" id="modalPrev">&#10094;</button>
      <img id="modalImage" src="" alt="Enlarged view" />
      <button class="modal-nav modal-next" id="modalNext">&#10095;</button>
      <div class="modal-label" id="modalLabel"></div>
    </div>
  </div>

  <script>
    const mappings = {{DATA_JSON}};
    const serveMode = {{SERVE_MODE}};

    let currentKey = null;
    let currentPreviewHtml = null;
    let isCommentDirty = false;
    let currentSensorFilter = 'all';
    let currentImages = [];
    let currentModalIndex = 0;
    let isResizing = false;
    let resizeData = null;

    function toggleExpand(btn) {
      const box = btn.closest('.box');
      const isExpanded = box.classList.contains('expanded');
      
      if (isExpanded) {
        box.classList.remove('expanded');
        btn.textContent = '⛶';
      } else {
        document.querySelectorAll('.box.expanded').forEach(b => {
          b.classList.remove('expanded');
          b.querySelector('.expand-btn').textContent = '⛶';
        });
        box.classList.add('expanded');
        btn.textContent = '✕';
      }
    }

    function initResize() {
      const layout = document.querySelector('.layout');
      const handles = document.querySelectorAll('.resize-handle');
      
      handles.forEach(handle => {
        handle.addEventListener('mousedown', (e) => {
          isResizing = true;
          const box = handle.closest('.box');
          const isVertical = handle.classList.contains('resize-handle-v');
          
          resizeData = {
            box: box,
            isVertical: isVertical,
            startX: e.clientX,
            startY: e.clientY,
            startWidth: box.offsetWidth,
            startHeight: box.offsetHeight
          };
          
          document.body.style.cursor = isVertical ? 'col-resize' : 'row-resize';
          e.preventDefault();
        });
      });
      
      document.addEventListener('mousemove', (e) => {
        if (!isResizing || !resizeData) return;
        
        const { box, isVertical, startX, startY, startWidth, startHeight } = resizeData;
        
        if (isVertical) {
          const deltaX = e.clientX - startX;
          const newWidth = Math.max(200, startWidth + deltaX);
          const percentage = (newWidth / layout.offsetWidth) * 100;
          box.style.gridColumn = `span 1`;
          box.style.width = percentage + '%';
        } else {
          const deltaY = e.clientY - startY;
          const newHeight = Math.max(150, startHeight + deltaY);
          const percentage = (newHeight / layout.offsetHeight) * 100;
          box.style.gridRow = `span 1`;
          box.style.height = percentage + '%';
        }
        
        e.preventDefault();
      });
      
      document.addEventListener('mouseup', () => {
        if (isResizing) {
          isResizing = false;
          resizeData = null;
          document.body.style.cursor = '';
        }
      });
    }

    function addStaggerAnimation() {
      // Removed for performance
    }

    function populateDropdown() {
      console.log('populateDropdown called');
      console.log('mappings:', mappings);
      const select = document.getElementById('logSelect');
      const keys = Object.keys(mappings).sort();
      console.log('keys:', keys);
      select.innerHTML = '';
      keys.forEach(function(k) {
        const opt = document.createElement('option');
        opt.value = k;
        opt.textContent = mappings[k].html_folder || k;
        select.appendChild(opt);
        console.log('Added option:', k, mappings[k].html_folder);
      });
      select.addEventListener('change', function() {
        updateScenario(select.value);
      });
      if (keys.length > 0) {
        updateScenario(keys[0]);
      }
    }

    function populateSensorDropdown(entry) {
      const select = document.getElementById('sensorSelect');
      select.innerHTML = '<option value="all">All</option>';
      
      if (!entry || !entry.images || typeof entry.images !== 'object') {
        return;
      }
      
      const sensorNames = Object.keys(entry.images).sort();
      sensorNames.forEach(function(sn) {
        const opt = document.createElement('option');
        opt.value = sn;
        opt.textContent = sn.charAt(0).toUpperCase() + sn.slice(1);
        select.appendChild(opt);
      });
      
      select.value = 'all';
      currentSensorFilter = 'all';
    }

    function buildSensorGrid(entry, filterSensor) {
      const grid = document.getElementById('sensorGrid');
      grid.innerHTML = '';
      currentImages = [];
      
      if (!entry || !entry.images || typeof entry.images !== 'object' || Object.keys(entry.images).length === 0) {
        const msg = document.createElement('div');
        msg.className = 'no-data';
        msg.textContent = 'No images for this scenario.';
        grid.appendChild(msg);
        return;
      }

      const positions = ['FL', 'FR', 'RL', 'RR', 'FC'];
      let used = 0;
      
      let sensorNames = Object.keys(entry.images).sort();
      if (filterSensor && filterSensor !== 'all') {
        sensorNames = sensorNames.filter(function(sn) { return sn === filterSensor; });
      }
      
      sensorNames.forEach(function(sensorName) {
        const positionsData = entry.images[sensorName];
        if (!positionsData || typeof positionsData !== 'object') return;
        
        positions.forEach(function(pos) {
          const imgPath = positionsData[pos];
          if (!imgPath) return;
          
          const imgIndex = currentImages.length;
          const label = sensorName + ' - ' + pos;
          currentImages.push({ src: imgPath, label: label });
          
          used += 1;
          const card = document.createElement('div');
          card.className = 'sensor-card';
          card.setAttribute('data-index', imgIndex);
          card.addEventListener('click', function() {
            openModal(imgIndex);
          });
          
          const img = document.createElement('img');
          img.src = imgPath;
          img.alt = imgPath.split('/').pop() || label;
          const labelDiv = document.createElement('div');
          labelDiv.textContent = label;
          card.appendChild(img);
          card.appendChild(labelDiv);
          grid.appendChild(card);
        });
      });

      if (used === 0) {
        const msg = document.createElement('div');
        msg.className = 'no-data';
        msg.textContent = filterSensor && filterSensor !== 'all' 
          ? 'No images for sensor: ' + filterSensor 
          : 'No FL/FR/RL/RR/FC images for this log.';
        grid.appendChild(msg);
      } else {
        addStaggerAnimation();
      }
    }

    function openModal(index) {
      if (currentImages.length === 0) return;
      currentModalIndex = index;
      updateModalContent();
      document.getElementById('imageModal').classList.add('active');
      document.body.style.overflow = 'hidden';
    }

    function closeModal() {
      document.getElementById('imageModal').classList.remove('active');
      document.body.style.overflow = '';
    }

    function updateModalContent() {
      const img = document.getElementById('modalImage');
      const label = document.getElementById('modalLabel');
      const prevBtn = document.getElementById('modalPrev');
      const nextBtn = document.getElementById('modalNext');
      
      if (currentImages.length === 0) return;
      
      const current = currentImages[currentModalIndex];
      img.src = current.src;
      label.textContent = current.label + ' (' + (currentModalIndex + 1) + '/' + currentImages.length + ')';
      
      prevBtn.style.visibility = currentModalIndex > 0 ? 'visible' : 'hidden';
      nextBtn.style.visibility = currentModalIndex < currentImages.length - 1 ? 'visible' : 'hidden';
    }

    function navigateModal(direction) {
      const newIndex = currentModalIndex + direction;
      if (newIndex >= 0 && newIndex < currentImages.length) {
        currentModalIndex = newIndex;
        updateModalContent();
      }
    }

    function getCommentPath(entry) {
      if (!entry || !entry.comment_path) return null;
      return entry.comment_path;
    }

    async function loadExistingComment(entry) {
      const commentBox = document.getElementById('commentBox');
      const pathLabel = document.getElementById('commentPath');
      const saveBtn = document.getElementById('saveCommentBtn');
      const editBtn = document.getElementById('editCommentBtn');
      const path = getCommentPath(entry);
      
      commentBox.readOnly = true;
      saveBtn.style.display = 'none';
      editBtn.style.display = 'inline-block';
      isCommentDirty = false;
      
      if (!path) {
        commentBox.value = '';
        pathLabel.textContent = 'No video selected.';
        pathLabel.classList.add('no-data');
        return;
      }

      pathLabel.textContent = path;
      pathLabel.classList.remove('no-data');
      
      // In serve mode, always fetch fresh content from server
      if (serveMode) {
        try {
          const response = await fetch('/api/get-comment', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ path: path })
          });
          const result = await response.json();
          if (result.success) {
            entry.comment_content = result.content;
            commentBox.value = result.content;
            return;
          }
        } catch (e) {
          console.warn('Could not fetch comment from server, using cached:', e);
        }
      }
      
      // Fallback to cached content
      commentBox.value = entry.comment_content || '';
    }

    async function saveComment(entry) {
      const commentBox = document.getElementById('commentBox');
      const saveBtn = document.getElementById('saveCommentBtn');
      const editBtn = document.getElementById('editCommentBtn');
      const path = getCommentPath(entry);
      if (!path) return;

      const body = commentBox.value || '';
      
      // Show saving state
      saveBtn.textContent = 'Saving...';
      saveBtn.disabled = true;
      
      try {
        const response = await fetch('/api/save-comment', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({
            path: path,
            content: body
          })
        });
        
        const result = await response.json();
        
        if (result.success) {
          // Update the local mapping immediately
          entry.comment_content = body;
          mappings[currentKey].comment_content = body;
          
          // Also update commentBox value to reflect saved content
          commentBox.value = body;
          
          alert('Comment saved successfully!');
          
          isCommentDirty = false;
          commentBox.readOnly = true;
          saveBtn.style.display = 'none';
          editBtn.style.display = 'inline-block';
        } else {
          alert('Failed to save: ' + (result.error || 'Unknown error'));
        }
      } catch (error) {
        // Fallback to download if server not running
        console.warn('Server not available, falling back to download:', error);
        
        const defaultFileName = path.split('/').pop() || 'comment.txt';
        const absPath = entry.comment_abs_path || path;
        
        const blob = new Blob([body], { type: 'text/plain' });
        const a = document.createElement('a');
        a.href = URL.createObjectURL(blob);
        a.download = defaultFileName;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(a.href);
        
        alert('Server not running. File downloaded.\\nCopy to: ' + absPath);
        
        isCommentDirty = false;
        commentBox.readOnly = true;
        saveBtn.style.display = 'none';
        editBtn.style.display = 'inline-block';
      } finally {
        saveBtn.textContent = 'Save';
        saveBtn.disabled = false;
      }
    }

    async function refreshComment(entry) {
      // Refresh comment content from server
      const path = getCommentPath(entry);
      if (!path) return;
      
      try {
        const response = await fetch('/api/get-comment', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ path: path })
        });
        const result = await response.json();
        if (result.success) {
          entry.comment_content = result.content;
          const commentBox = document.getElementById('commentBox');
          commentBox.value = result.content;
        }
      } catch (e) {
        console.warn('Could not refresh comment:', e);
      }
    }

    function updateHtmlPreview(entry, key) {
      const iframe = document.getElementById('htmlPreview');
      const label = document.getElementById('htmlPreviewLabel');
      const openBtn = document.getElementById('openHtmlBtn');
      const htmlInfo = document.getElementById('htmlInfo');

      if (entry && Array.isArray(entry.html_files) && entry.html_files.length > 0) {
        const mainHtml = entry.html_files[0];
        currentPreviewHtml = mainHtml;
        iframe.src = mainHtml;
        label.textContent = mainHtml.split('/').pop() || 'HTML preview';
        label.classList.remove('no-data');
        openBtn.disabled = false;
        if (htmlInfo) {
          htmlInfo.textContent = entry.html_folder || key;
          htmlInfo.classList.remove('no-data');
        }
      } else {
        currentPreviewHtml = null;
        iframe.removeAttribute('src');
        label.textContent = 'No HTML preview.';
        label.classList.add('no-data');
        openBtn.disabled = true;
        if (htmlInfo) {
          htmlInfo.textContent = 'No HTML files mapped for this scenario.';
          htmlInfo.classList.add('no-data');
        }
      }
    }

    function wireGlobalHandlers() {
      const openBtn = document.getElementById('openHtmlBtn');
      openBtn.addEventListener('click', function() {
        if (currentPreviewHtml) {
          window.open(currentPreviewHtml, '_blank');
        }
      });

      const commentBox = document.getElementById('commentBox');
      const saveBtn = document.getElementById('saveCommentBtn');
      const editBtn = document.getElementById('editCommentBtn');
      
      editBtn.addEventListener('click', function() {
        commentBox.readOnly = false;
        commentBox.focus();
        editBtn.style.display = 'none';
        saveBtn.style.display = 'inline-block';
      });
      
      commentBox.addEventListener('input', function() {
        isCommentDirty = true;
      });
      
      saveBtn.addEventListener('click', function() {
        if (!currentKey) return;
        const entry = mappings[currentKey];
        saveComment(entry);
      });

      const sensorSelect = document.getElementById('sensorSelect');
      sensorSelect.addEventListener('change', function() {
        currentSensorFilter = sensorSelect.value;
        if (currentKey) {
          const entry = mappings[currentKey];
          buildSensorGrid(entry, currentSensorFilter);
        }
      });

      document.getElementById('modalClose').addEventListener('click', closeModal);
      document.getElementById('modalPrev').addEventListener('click', function() {
        navigateModal(-1);
      });
      document.getElementById('modalNext').addEventListener('click', function() {
        navigateModal(1);
      });
      
      document.getElementById('imageModal').addEventListener('click', function(e) {
        if (e.target === this) {
          closeModal();
        }
      });
      
      document.addEventListener('keydown', function(e) {
        const modal = document.getElementById('imageModal');
        if (!modal.classList.contains('active')) return;
        
        if (e.key === 'Escape') {
          closeModal();
        } else if (e.key === 'ArrowLeft') {
          navigateModal(-1);
        } else if (e.key === 'ArrowRight') {
          navigateModal(1);
        }
      });
    }

    async function updateScenario(key) {
      currentKey = key;
      const entry = mappings[key];
      const videoPlayer = document.getElementById('videoPlayer');
      const videoInfo = document.getElementById('videoInfo');

      if (entry && entry.video) {
        videoPlayer.src = entry.video;
        videoPlayer.autoplay = true;
        videoPlayer.load();
        videoPlayer.play().catch(function() {});
        videoInfo.textContent = entry.video_name || entry.video;
        videoInfo.classList.remove('no-data');
      } else {
        videoPlayer.removeAttribute('src');
        videoInfo.textContent = 'No video for this scenario.';
        videoInfo.classList.add('no-data');
      }

      populateSensorDropdown(entry);
      buildSensorGrid(entry, 'all');
      await loadExistingComment(entry);
      updateHtmlPreview(entry, key);
    }

    // Cluster connection functions
    function openClusterModal() {
      document.getElementById('clusterModal').classList.add('active');
      checkClusterStatus();
    }
    
    function closeClusterModal() {
      document.getElementById('clusterModal').classList.remove('active');
    }
    
    function showClusterMessage(msg, type) {
      const el = document.getElementById('clusterMessage');
      el.textContent = msg;
      el.className = 'cluster-message ' + type;
    }
    
    function updateClusterUI(connected, serverName) {
      const status = document.getElementById('clusterStatus');
      const connectBtn = document.getElementById('clusterConnectBtn');
      const disconnectBtn = document.getElementById('clusterDisconnectBtn');
      const pathSection = document.getElementById('pathSection');
      
      if (connected) {
        status.className = 'cluster-status connected';
        status.querySelector('.status-text').textContent = 'Connected to ' + serverName;
        connectBtn.style.display = 'none';
        disconnectBtn.style.display = 'block';
        pathSection.style.display = 'block';
      } else {
        status.className = 'cluster-status disconnected';
        status.querySelector('.status-text').textContent = 'Disconnected';
        connectBtn.style.display = 'block';
        disconnectBtn.style.display = 'none';
        pathSection.style.display = 'none';
      }
    }
    
    async function checkClusterStatus() {
      try {
        const resp = await fetch('/api/cluster/status');
        const data = await resp.json();
        updateClusterUI(data.connected, data.server);
      } catch (e) {
        updateClusterUI(false, null);
      }
    }
    
    async function connectCluster() {
      const server = document.getElementById('clusterServer').value;
      const username = document.getElementById('clusterUser').value;
      const password = document.getElementById('clusterPass').value;
      
      if (!username || !password) {
        showClusterMessage('Please enter username and password', 'error');
        return;
      }
      
      const status = document.getElementById('clusterStatus');
      status.className = 'cluster-status connecting';
      status.querySelector('.status-text').textContent = 'Connecting...';
      
      try {
        const resp = await fetch('/api/cluster/connect', {
          method: 'POST',
          headers: {'Content-Type': 'application/json'},
          body: JSON.stringify({server, username, password})
        });
        const data = await resp.json();
        
        if (data.success) {
          updateClusterUI(true, server);
          showClusterMessage(data.message, 'success');
          document.getElementById('clusterPass').value = '';
        } else {
          updateClusterUI(false, null);
          showClusterMessage(data.error || data.message, 'error');
        }
      } catch (e) {
        updateClusterUI(false, null);
        showClusterMessage('Connection failed: ' + e.message, 'error');
      }
    }
    
    async function disconnectCluster() {
      try {
        await fetch('/api/cluster/disconnect', {method: 'POST'});
        updateClusterUI(false, null);
        showClusterMessage('Disconnected successfully', 'info');
      } catch (e) {
        showClusterMessage('Disconnect error: ' + e.message, 'error');
      }
    }
    
    function togglePassword() {
      const input = document.getElementById('clusterPass');
      const btn = document.querySelector('.password-toggle');
      if (input.type === 'password') {
        input.type = 'text';
        btn.textContent = '🙈';
      } else {
        input.type = 'password';
        btn.textContent = '👁';
      }
    }
    
    async function downloadFromCluster() {
      const htmlPath = document.getElementById('htmlPathInput').value;
      const videoPath = document.getElementById('videoPathInput').value;
      const localPath = document.getElementById('localPathInput').value || 'db';
      const server = document.getElementById('clusterServer').value;
      const requiredPrefix = server === 'southfield' ? '/mnt' : '/net';
      const invalidPaths = [];

      if (!htmlPath.startsWith(requiredPrefix)) {
        invalidPaths.push('HTML Path must start with ' + requiredPrefix);
      }
      if (!videoPath.startsWith(requiredPrefix)) {
        invalidPaths.push('Video Path must start with ' + requiredPrefix);
      }

      if (invalidPaths.length > 0) {
        alert(invalidPaths.join("\\n"));
        showClusterMessage(invalidPaths.join(' '), 'error');
        return;
      }
      
      // Show progress container
      const progressContainer = document.getElementById('progressContainer');
      const progressBar = document.getElementById('progressBar');
      const progressText = document.getElementById('progressText');
      const progressPhase = document.getElementById('progressPhase');
      const progressFile = document.getElementById('progressFile');
      
      progressContainer.classList.add('active');
      progressBar.classList.add('indeterminate');
      progressBar.style.width = '100%';
      progressText.textContent = '';
      progressPhase.textContent = 'Starting download...';
      progressFile.textContent = '';
      showClusterMessage('', '');
      
      // Start progress polling
      let progressInterval = setInterval(async () => {
        try {
          const pResp = await fetch('/api/cluster/progress');
          const progress = await pResp.json();
          
          if (progress.phase === 'counting') {
            progressPhase.textContent = '📊 Scanning remote directories...';
            progressFile.textContent = progress.message || '';
          } else if (progress.phase === 'downloading') {
            progressBar.classList.remove('indeterminate');
            const pct = progress.total_files > 0 
              ? Math.round((progress.downloaded_files / progress.total_files) * 100) 
              : 0;
            progressBar.style.width = pct + '%';
            progressText.textContent = pct + '%';
            progressPhase.textContent = '📥 Downloading: ' + progress.downloaded_files + ' / ' + progress.total_files + ' files';
            progressFile.textContent = progress.current_file || '';
          } else if (progress.phase === 'processing') {
            progressBar.style.width = '100%';
            progressText.textContent = '100%';
            progressPhase.textContent = '⚙️ Building viewer HTML...';
            progressFile.textContent = '';
          } else if (progress.phase === 'complete' || progress.phase === 'error') {
            clearInterval(progressInterval);
          }
        } catch (e) {
          // Ignore polling errors
        }
      }, 500);
      
      try {
        const resp = await fetch('/api/cluster/download', {
          method: 'POST',
          headers: {'Content-Type': 'application/json'},
          body: JSON.stringify({html_path: htmlPath, video_path: videoPath, local_path: localPath})
        });
        const data = await resp.json();
        
        clearInterval(progressInterval);
        
        if (data.success) {
          progressBar.classList.remove('indeterminate');
          progressBar.style.width = '100%';
          progressText.textContent = '100%';
          progressPhase.textContent = '✅ Download complete!';
          progressFile.textContent = '';
          showClusterMessage('Download complete! Saved to: ' + localPath + '. Reloading...', 'success');
          setTimeout(() => {
            closeClusterModal();
            progressContainer.classList.remove('active');
            window.location.reload();
          }, 1500);
        } else {
          progressBar.classList.remove('indeterminate');
          progressBar.style.width = '0%';
          progressPhase.textContent = '❌ Download failed';
          progressFile.textContent = '';
          showClusterMessage(data.error, 'error');
          setTimeout(() => {
            progressContainer.classList.remove('active');
          }, 3000);
        }
      } catch (e) {
        clearInterval(progressInterval);
        progressBar.classList.remove('indeterminate');
        progressBar.style.width = '0%';
        progressPhase.textContent = '❌ Download failed';
        progressFile.textContent = '';
        showClusterMessage('Download failed: ' + e.message, 'error');
        setTimeout(() => {
          progressContainer.classList.remove('active');
        }, 3000);
      }
    }

    document.addEventListener('DOMContentLoaded', function() {
      console.log('DOMContentLoaded fired');
      console.log('serveMode:', serveMode);
      wireGlobalHandlers();
      populateDropdown();
      initResize();
      
      // Show message if no logs available
      const keys = Object.keys(mappings);
      if (keys.length === 0) {
        const select = document.getElementById('logSelect');
        const opt = document.createElement('option');
        opt.value = '';
        opt.textContent = serveMode ? 'No logs - Click "Go Online" to download' : 'No logs found';
        opt.disabled = true;
        select.appendChild(opt);
      }
    });
  </script>
</body>
</html>
"""
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(html_template.replace("{{DATA_JSON}}", data_json).replace("{{SERVE_MODE}}", serve_mode_js))
