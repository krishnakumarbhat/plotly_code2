css = """body { 
                    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
                    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
                    margin: 0; 
                    padding: 2em; 
                    color: #333;
                    min-height: 100vh;
}
.container {
                    max-width: 1200px;
                    margin: 0 auto;
                    background: rgba(255, 255, 255, 0.95);
                    border-radius: 20px;
                    box-shadow: 0 20px 40px rgba(0,0,0,0.1);
                    overflow: hidden;
}
.header {
                    background: linear-gradient(135deg, #2a357a 0%, #4a5568 100%);
                    color: white;
                    padding: 2em;
                    text-align: center;
}
.header h1 {
                    margin: 0;
                    font-size: 2.5em;
                    font-weight: 300;
                    letter-spacing: 2px;
}
.content {
                    padding: 2em;
}
.category-section {
                    background: #f8f9fa;
                    border-radius: 15px;
                    padding: 1.5em;
                    border-left: 5px solid #2a357a;
}
.category-title {
                    font-size: 1.4em;
                    font-weight: 600;
                    color: #2a357a;
                    margin-bottom: 1em;
}
.plot-links {
                    display: grid;
                    grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
                    gap: 1em;
}
.plot-link {
                    background: white;
                    padding: 1em;
                    border-radius: 10px;
                    text-decoration: none;
                    color: #2a357a;
                    border: 2px solid #e2e8f0;
                    transition: all 0.3s ease;
}
.plot-link:hover {
                    background: #2a357a;
                    color: white;
                    border-color: #2a357a;
}
.metadata {
                    background: #e8f4fd;
                    padding: 1em;
                    border-radius: 10px;
                    margin-bottom: 2em;
                    text-align: center;
}
.metadata-grid {
                    display: grid;
                    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
                    gap: 1em;
}
.metadata-item {
                    background: white;
                    padding: 0.5em;
                    border-radius: 5px;
                    font-size: 0.9em;
}
/* Tab-based interface */
.sensors-section {
    background: #f8f9fa;
    border-radius: 15px;
    padding: 1.5em;
    margin-bottom: 1.5em;
    border-left: 5px solid #2a357a;
}
.sensors-title {
    font-size: 1.6em;
    font-weight: 600;
    color: #2a357a;
    margin-bottom: 1em;
}
.sensor-tabs {
    display: flex;
    gap: 0.5em;
    margin-bottom: 1.5em;
    flex-wrap: wrap;
}
.sensor-tab {
    background: #e2e8f0;
    border: 2px solid transparent;
    padding: 0.8em 1.5em;
    border-radius: 25px;
    cursor: pointer;
    font-weight: 500;
    color: #4a5568;
    transition: all 0.3s ease;
    text-decoration: none;
    display: inline-block;
}
.sensor-tab:hover {
    background: #cbd5e0;
    transform: translateY(-2px);
}
.sensor-tab.active {
    background: #2a357a;
    color: white;
    border-color: #1a237e;
}
.streams-section {
    background: white;
    border-radius: 10px;
    padding: 1em;
    margin-bottom: 1em;
    border-left: 3px solid #4a5568;
}
.streams-title {
    font-size: 1.3em;
    font-weight: 500;
    color: #4a5568;
    margin-bottom: 0.5em;
}
.stream-tabs {
    display: flex;
    gap: 0.5em;
    margin-bottom: 1.5em;
    flex-wrap: wrap;
}
.stream-tab {
    background: #f7fafc;
    border: 2px solid #e2e8f0;
    padding: 0.6em 1.2em;
    border-radius: 20px;
    cursor: pointer;
    font-weight: 500;
    color: #4a5568;
    transition: all 0.3s ease;
    text-decoration: none;
    display: inline-block;
}
.stream-tab:hover {
    background: #edf2f7;
    border-color: #cbd5e0;
    transform: translateY(-1px);
}
.stream-tab.active {
    background: #4a5568;
    color: white;
    border-color: #2d3748;
}
.plots-content {
    background: white;
    border-radius: 15px;
    padding: 1.5em;
    min-height: 200px;
    border: 2px solid #e2e8f0;
}
.plots-content:empty::before {
    content: "Select a sensor and stream to view plots";
    color: #a0aec0;
    font-style: italic;
    text-align: center;
    display: block;
    padding: 2em;
}
.plot-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 1.5em;
}
.plot-card {
    background: #f8f9fa;
    border-radius: 10px;
    padding: 1.5em;
    border-left: 4px solid #2a357a;
    transition: all 0.3s ease;
}
.plot-card:hover {
    transform: translateY(-2px);
    box-shadow: 0 4px 15px rgba(0,0,0,0.1);
}
.plot-card h4 {
    color: #2a357a;
    margin: 0 0 0.5em 0;
    font-size: 1.2em;
}
.plot-card p {
    color: #4a5568;
    margin: 0;
    font-size: 0.9em;
}
.plot-card .plot-link {
    display: inline-block;
    background: #2a357a;
    color: white;
    padding: 0.5em 1em;
    border-radius: 5px;
    text-decoration: none;
    margin-top: 0.5em;
    transition: all 0.3s ease;
}
.plot-card .plot-link:hover {
    background: #1a237e;
    transform: translateY(-1px);
}
"""

# Additional CSS for master index and other components
master_index_css = """
body { 
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    margin: 0; 
    padding: 2em; 
    color: #333;
    min-height: 100vh;
}
.container {
    max-width: 1400px;
    margin: 0 auto;
    background: rgba(255, 255, 255, 0.95);
    border-radius: 20px;
    box-shadow: 0 20px 40px rgba(0,0,0,0.1);
    overflow: hidden;
}
.header {
    background: linear-gradient(135deg, #2a357a 0%, #4a5568 100%);
    color: white;
    padding: 2em;
    text-align: center;
}
.header h1 {
    margin: 0;
    font-size: 2.5em;
    font-weight: 300;
    letter-spacing: 2px;
}
.content {
    padding: 2em;
}
.metadata {
    background: #e8f4fd;
    padding: 1em;
    border-radius: 10px;
    margin-bottom: 2em;
    text-align: center;
}
.metadata-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 1em;
}
.metadata-item {
    background: white;
    padding: 0.5em;
    border-radius: 5px;
    font-size: 0.9em;
}
.sensor-section {
    background: #f8f9fa;
    border-radius: 15px;
    padding: 1.5em;
    margin-bottom: 1.5em;
    border-left: 5px solid #2a357a;
}
.sensor-title {
    font-size: 1.6em;
    font-weight: 600;
    color: #2a357a;
    margin-bottom: 1em;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: space-between;
}
.sensor-title:hover {
    color: #1a237e;
}
.sensor-content {
    display: none;
    margin-top: 1em;
}
.sensor-content.active {
    display: block;
}
.stream-section {
    background: white;
    border-radius: 10px;
    padding: 1em;
    margin-bottom: 1em;
    border-left: 3px solid #4a5568;
}
.stream-title {
    font-size: 1.3em;
    font-weight: 500;
    color: #4a5568;
    margin-bottom: 0.5em;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: space-between;
}
.stream-title:hover {
    color: #2a357a;
}
.stream-content {
    display: none;
    margin-top: 0.5em;
}
.stream-content.active {
    display: block;
}
.category-section {
    background: #f1f3f4;
    border-radius: 8px;
    padding: 0.8em;
    margin-bottom: 0.8em;
}
.category-title {
    font-size: 1.1em;
    font-weight: 500;
    color: #2a357a;
    margin-bottom: 0.5em;
}
.plot-links {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 0.5em;
}
.plot-link {
    background: white;
    padding: 0.8em;
    border-radius: 6px;
    text-decoration: none;
    color: #2a357a;
    border: 1px solid #e2e8f0;
    transition: all 0.3s ease;
    font-size: 0.9em;
}
.plot-link:hover {
    background: #2a357a;
    color: white;
    border-color: #2a357a;
    transform: translateY(-2px);
}
.toggle-icon {
    font-size: 1.2em;
    transition: transform 0.3s ease;
}
.toggle-icon.rotated {
    transform: rotate(90deg);
}
"""

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