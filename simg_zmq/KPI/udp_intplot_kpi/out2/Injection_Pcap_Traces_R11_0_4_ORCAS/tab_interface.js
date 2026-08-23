let currentSensor = null;
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
        return '<a href="#" role="button" class="stream-tab" onclick="return selectStream(this, '' + streamName + '')">' + streamName + '</a>';
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
});