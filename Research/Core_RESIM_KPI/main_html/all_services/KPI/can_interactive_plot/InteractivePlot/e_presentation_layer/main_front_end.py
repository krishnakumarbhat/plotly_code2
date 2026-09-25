css = """:root{--page-bg:#eef3f7;--panel-bg:#ffffff;--panel-muted:#f7fafc;--panel-border:#d9e3ec;--text-main:#17324a;--text-muted:#617789;--accent:#175f7b;--accent-strong:#0f4760;--shadow:0 10px 24px rgba(19,45,69,.08)}*{box-sizing:border-box}body{background:var(--page-bg);font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;margin:0;padding:20px;color:var(--text-main);min-height:100vh}.container{max-width:1400px;margin:0 auto;background:var(--panel-bg);border-radius:20px;box-shadow:var(--shadow);overflow:hidden;border:1px solid var(--panel-border)}.header{background:linear-gradient(180deg,#1c5972 0%,#17324a 100%);color:#fff;padding:24px 28px;text-align:center}.header h1{margin:0;font-size:2.2rem;font-weight:600;letter-spacing:.01em}.content{padding:20px}.metadata{background:#f6fbfd;padding:14px;border:1px solid var(--panel-border);border-radius:16px;margin-bottom:20px;text-align:center}.metadata-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:10px}.metadata-item{background:#fff;padding:10px 12px;border-radius:12px;font-size:.92rem;border:1px solid var(--panel-border)}.sensors-section,.sensor-section,.category-section{background:var(--panel-muted);border-radius:16px;padding:16px;border:1px solid var(--panel-border)}.sensors-title,.sensor-title,.category-title{color:var(--accent-strong)}.sensors-title{font-size:1.3rem;font-weight:700;margin-bottom:12px}.sensor-tabs,.stream-tabs{display:flex;gap:8px;margin-bottom:14px;flex-wrap:wrap}.sensor-tab,.stream-tab{background:#fff;border:1px solid var(--panel-border);padding:10px 14px;border-radius:999px;cursor:pointer;font-weight:600;color:var(--text-main);transition:background .16s ease,border-color .16s ease,color .16s ease;text-decoration:none;display:inline-block}.sensor-tab:hover,.stream-tab:hover{background:#f0f7fa;border-color:#bfd4df}.sensor-tab.active,.stream-tab.active{background:var(--accent);color:#fff;border-color:var(--accent)}.streams-section,.stream-section{background:#fff;border-radius:14px;padding:14px;margin-bottom:12px;border:1px solid var(--panel-border)}.streams-title,.stream-title{font-size:1.05rem;font-weight:700;color:var(--text-main);margin-bottom:8px}.plots-content{background:#fff;border-radius:16px;padding:16px;min-height:180px;border:1px solid var(--panel-border)}.plots-content:empty::before{content:'Select a sensor and stream to view plots';color:var(--text-muted);font-style:italic;text-align:center;display:block;padding:2em}.plot-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:14px}.plot-card{background:#f9fcfd;border-radius:14px;padding:16px;border:1px solid var(--panel-border);min-width:0}.plot-card h4{color:var(--accent-strong);margin:0 0 8px;font-size:1.05rem}.plot-card p{color:var(--text-muted);margin:0 0 10px;font-size:.92rem}.plot-card .plot-link{display:inline-block;background:var(--accent);color:#fff;padding:8px 12px;border-radius:999px;text-decoration:none;transition:background .16s ease}.plot-card .plot-link:hover{background:var(--accent-strong)}@media (max-width:920px){body{padding:14px}.plot-grid{grid-template-columns:1fr}}"""

# Additional CSS for master index and other components
master_index_css = """:root{--page-bg:#eef3f7;--panel-bg:#ffffff;--panel-muted:#f7fafc;--panel-border:#d9e3ec;--text-main:#17324a;--text-muted:#617789;--accent:#175f7b;--accent-strong:#0f4760;--shadow:0 10px 24px rgba(19,45,69,.08)}*{box-sizing:border-box}body{background:var(--page-bg);font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;margin:0;padding:20px;color:var(--text-main);min-height:100vh}.container{max-width:1440px;margin:0 auto;background:var(--panel-bg);border-radius:20px;box-shadow:var(--shadow);overflow:hidden;border:1px solid var(--panel-border)}.header{background:linear-gradient(180deg,#1c5972 0%,#17324a 100%);color:#fff;padding:24px 28px;text-align:center}.header h1{margin:0;font-size:2.2rem;font-weight:600}.content{padding:20px}.metadata{background:#f6fbfd;padding:14px;border:1px solid var(--panel-border);border-radius:16px;margin-bottom:20px;text-align:center}.metadata-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:10px}.metadata-item{background:#fff;padding:10px 12px;border-radius:12px;font-size:.92rem;border:1px solid var(--panel-border)}.sensor-section{background:var(--panel-muted);border-radius:16px;padding:16px;margin-bottom:14px;border:1px solid var(--panel-border)}.sensor-title{font-size:1.05rem;font-weight:700;color:var(--accent-strong);cursor:pointer;display:flex;align-items:flex-start;justify-content:space-between;gap:12px}.sensor-summary{display:block;flex:1;line-height:1.3}.sensor-summary-primary{display:block}.sensor-summary-secondary{display:block;margin-top:4px;font-size:.92rem;font-weight:600;color:var(--text-main)}.sensor-content{display:none;margin-top:12px}.sensor-content.active{display:block}.stream-section{background:#fff;border-radius:14px;padding:14px;margin-bottom:12px;border:1px solid var(--panel-border)}.stream-title{font-size:1rem;font-weight:700;color:var(--text-main);cursor:pointer;display:flex;align-items:center;justify-content:space-between;gap:12px}.stream-content{display:none;margin-top:10px}.stream-content.active{display:block}.category-section{background:#f9fcfd;border-radius:14px;padding:12px;margin-bottom:10px;border:1px solid var(--panel-border)}.category-title{font-size:1rem;font-weight:700;color:var(--accent-strong);margin-bottom:10px}.plot-links{display:grid;grid-template-columns:repeat(auto-fit,minmax(220px,1fr));gap:8px}.plot-link{background:#fff;padding:10px 12px;border-radius:12px;text-decoration:none;color:var(--accent-strong);border:1px solid var(--panel-border);transition:background .16s ease,border-color .16s ease;font-size:.92rem}.plot-link:hover{background:#eef7fb;border-color:#bfd4df}.toggle-icon{font-size:1rem;transition:transform .18s ease}.toggle-icon.rotated{transform:rotate(90deg)}@media (max-width:920px){body{padding:14px}}"""

# Additional JavaScript for master index
master_index_js = """
function toggleSection(element) {
    const content = element.nextElementSibling;
    const icon = element.querySelector('.toggle-icon');
    
    if (content.classList.contains('active')) {
        content.classList.remove('active');
        icon.classList.remove('rotated');
    } else {
        content.classList.add('active');
        icon.classList.add('rotated');
    }
}

// Auto-expand first sensor by default
document.addEventListener('DOMContentLoaded', function() {
    const firstSensor = document.querySelector('.sensor-title');
    if (firstSensor) {
        toggleSection(firstSensor);
    }
});
"""

js = """let currentSensor = null;
let currentStream = null;

function selectSensor(el, sensorName) {
    if (el && el.preventDefault) { el.preventDefault(); }
    // Update sensor tab states
    document.querySelectorAll('.sensor-tab').forEach(tab => {
        tab.classList.remove('active');
    });
    if (el && el.classList) { el.classList.add('active'); }
    
    currentSensor = sensorName;
    currentStream = null;
    
    // Update stream tabs
    updateStreamTabs();
    
    // Clear plots content
    document.querySelector('.plots-content').innerHTML = '';
    
    // Show streams section
    document.querySelector('.streams-section').style.display = 'block';
    return false;
}

function selectStream(el, streamName) {
    if (el && el.preventDefault) { el.preventDefault(); }
    // Update stream tab states
    document.querySelectorAll('.stream-tab').forEach(tab => {
        tab.classList.remove('active');
    });
    if (el && el.classList) { el.classList.add('active'); }
    
    currentStream = streamName;
    
    // Load plots for selected sensor and stream
    loadPlots();
    return false;
}

function updateStreamTabs() {
    const streamTabsContainer = document.querySelector('.stream-tabs');
    const streams = getStreamsForSensor(currentSensor);
    
    streamTabsContainer.innerHTML = streams.map(function(streamName) {
        return '<a href="#" role="button" class="stream-tab" onclick="return selectStream(this, \'' + streamName + '\')">' + streamName + '</a>';
    }).join('');
}

function getStreamsForSensor(sensor) {
    // This will be populated with actual stream data
    return ['DOWN_SELECTION_STREAM'];
}

function loadPlots() {
    if (!currentSensor || !currentStream) return;
    
    const plotsContainer = document.querySelector('.plots-content');
    
    // Load plots based on selected sensor and stream
    const plotData = getPlotDataForSensorAndStream(currentSensor, currentStream);
    if (plotData && plotData.length > 0) {
        plotsContainer.innerHTML = generatePlotsHTML(plotData);
    } else {
        plotsContainer.innerHTML = '<p>No plots available for ' + currentSensor + ' - ' + currentStream + '</p>';
    }
}

function getPlotDataForSensorAndStream(sensor, stream) {
    // Get actual plot data from the page
    const plotData = window.plotData || {};
    const sensorData = plotData[sensor] || {};
    const streamData = sensorData[stream] || [];
    
    if (streamData.length === 0) {
        // Fallback to sample data if no real data
        return [
            { category: 'histogram', count: 1, name: sensor + ' histogram' },
            { category: 'general', count: 1, name: sensor + ' general' },
            { category: 'mismatch', count: 1, name: sensor + ' mismatch' }
        ];
    }
    
    return streamData;
}

function generatePlotsHTML(plotData) {
    let html = '<div class="plot-grid">';
    plotData.forEach(function(plot) {
        html += '<div class="plot-card">';
        html += '<h4>' + plot.category.charAt(0).toUpperCase() + plot.category.slice(1) + ' (' + plot.count + ' plots)</h4>';
        if (plot.file_path) {
            html += '<a href="' + plot.file_path + '" class="plot-link">' + plot.name + '</a>';
        } else {
            html += '<p>' + plot.name + '</p>';
        }
        html += '</div>';
    });
    html += '</div>';
    return html;
}

// Initialize with first sensor selected by default
document.addEventListener('DOMContentLoaded', function() {
    // Auto-select first sensor
    const firstSensorTab = document.querySelector('.sensor-tab');
    if (firstSensorTab) {
        const sensor = firstSensorTab.getAttribute('data-sensor') || firstSensorTab.textContent.trim();
        selectSensor(firstSensorTab, sensor);
    }
});"""