/**
 * File Browser Module for HPC Tools Platform
 */

let currentPath = '/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/2-Sim/USER_DATA';
let selectedFile = null;
let fileBrowserModal = null;

document.addEventListener('DOMContentLoaded', function() {
    const modalEl = document.getElementById('fileBrowserModal');
    if (modalEl) {
        fileBrowserModal = new bootstrap.Modal(modalEl);
        
        const selectBtn = document.getElementById('selectFileBtn');
        if (selectBtn) {
            selectBtn.addEventListener('click', function() {
                if (selectedFile) {
                    document.getElementById('input_path').value = selectedFile;
                    fileBrowserModal.hide();
                }
            });
        }
    }
});

function openFileBrowser() {
    selectedFile = null;
    const selectBtn = document.getElementById('selectFileBtn');
    if (selectBtn) selectBtn.disabled = true;
    
    loadDirectory(currentPath);
    if (fileBrowserModal) {
        fileBrowserModal.show();
    }
}

async function loadDirectory(path) {
    const content = document.getElementById('fileBrowserContent');
    if (!content) return;
    
    content.innerHTML = `
        <div class="text-center py-4">
            <div class="spinner-border text-primary" role="status">
                <span class="visually-hidden">Loading...</span>
            </div>
        </div>
    `;
    
    try {
        const response = await fetch(`/api/browse?path=${encodeURIComponent(path)}&extensions=h5,hdf5,mf4,csv,json`);
        const data = await response.json();
        
        if (data.error) {
            content.innerHTML = `<div class="alert alert-danger">${data.error}</div>`;
            return;
        }
        
        currentPath = data.current_path;
        renderBreadcrumb(currentPath);
        renderFileList(data, content);
    } catch (error) {
        console.error('Error loading directory:', error);
        content.innerHTML = '<div class="alert alert-danger">Error loading directory</div>';
    }
}

function renderBreadcrumb(path) {
    const breadcrumb = document.getElementById('breadcrumbList');
    if (!breadcrumb) return;
    
    const parts = path.split('/').filter(p => p);
    breadcrumb.innerHTML = '';
    
    let currentFullPath = '';
    parts.forEach((part, index) => {
        currentFullPath += '/' + part;
        const li = document.createElement('li');
        li.className = 'breadcrumb-item';
        
        if (index === parts.length - 1) {
            li.classList.add('active');
            li.textContent = part;
        } else {
            const pathCopy = currentFullPath;
            li.innerHTML = `<a href="#" onclick="loadDirectory('${pathCopy}'); return false;">${part}</a>`;
        }
        breadcrumb.appendChild(li);
    });
}

function renderFileList(data, container) {
    let html = '<div class="list-group">';
    
    // Parent directory link
    if (data.parent) {
        html += `
            <a href="#" class="list-group-item list-group-item-action" onclick="loadDirectory('${data.parent}'); return false;">
                <i class="bi bi-arrow-up me-2"></i>..
            </a>
        `;
    }
    
    // Directories
    data.directories.forEach(dir => {
        html += `
            <a href="#" class="list-group-item list-group-item-action" onclick="loadDirectory('${dir.path}'); return false;">
                <i class="bi bi-folder-fill text-warning me-2"></i>${dir.name}
            </a>
        `;
    });
    
    // Files
    data.files.forEach(file => {
        html += `
            <a href="#" class="list-group-item list-group-item-action file-item" 
               data-path="${file.path}" onclick="selectFilePath(this, '${file.path}'); return false;">
                <div class="d-flex justify-content-between align-items-center">
                    <span><i class="bi bi-file-earmark text-primary me-2"></i>${file.name}</span>
                    <small class="text-muted">${formatSize(file.size)}</small>
                </div>
            </a>
        `;
    });
    
    if (data.directories.length === 0 && data.files.length === 0) {
        html += '<div class="list-group-item text-muted text-center">No matching files found</div>';
    }
    
    html += '</div>';
    container.innerHTML = html;
}

function selectFilePath(element, path) {
    document.querySelectorAll('.file-item').forEach(el => {
        el.classList.remove('active');
    });
    
    element.classList.add('active');
    selectedFile = path;
    
    const selectBtn = document.getElementById('selectFileBtn');
    if (selectBtn) selectBtn.disabled = false;
}

function formatSize(bytes) {
    if (bytes === 0) return '0 B';
    const k = 1024;
    const sizes = ['B', 'KB', 'MB', 'GB', 'TB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(1)) + ' ' + sizes[i];
}
