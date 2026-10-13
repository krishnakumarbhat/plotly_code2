# This code is structured to generate the binary for the application.

minfy_js = """document.addEventListener('DOMContentLoaded', function() {
    const body = document.body;
    const overlay = document.getElementById('plot-overlay');
    const overlayContent = document.getElementById('plot-overlay-content');
    let expandedCard = null;

    function resizePlot(card) {
        if (!card || typeof Plotly === 'undefined') {
            return;
        }
        const graph = card.querySelector('.js-plotly-plot');
        if (graph) {
            requestAnimationFrame(function() {
                Plotly.Plots.resize(graph);
            });
            setTimeout(function() {
                Plotly.Plots.resize(graph);
            }, 90);
        }
    }

    function restoreCard(card) {
        if (!card || !card._placeholder) {
            return;
        }
        card.classList.remove('expanded');
        const button = card.querySelector('.plot-expand-btn');
        if (button) {
            button.textContent = 'Expand';
            button.setAttribute('aria-expanded', 'false');
        }
        card._placeholder.replaceWith(card);
        card._placeholder = null;
        resizePlot(card);
    }

    function closeExpandedPlot() {
        if (!expandedCard) {
            return;
        }
        restoreCard(expandedCard);
        overlay.classList.remove('visible');
        body.classList.remove('modal-open');
        overlayContent.innerHTML = '';
        expandedCard = null;
    }

    document.querySelectorAll('.plot-shell').forEach(function(card, idx) {
        card.style.animationDelay = (idx * 24) + 'ms';
        const button = card.querySelector('.plot-expand-btn');
        if (!button) {
            return;
        }
        button.addEventListener('click', function(event) {
            event.preventDefault();
            event.stopPropagation();
            if (expandedCard === card) {
                closeExpandedPlot();
                return;
            }
            if (expandedCard) {
                closeExpandedPlot();
            }
            const placeholder = document.createElement('div');
            placeholder.className = 'plot-placeholder';
            card._placeholder = placeholder;
            card.parentNode.insertBefore(placeholder, card);
            expandedCard = card;
            card.classList.add('expanded');
            button.textContent = 'Back';
            button.setAttribute('aria-expanded', 'true');
            overlayContent.appendChild(card);
            overlay.classList.add('visible');
            body.classList.add('modal-open');
            resizePlot(card);
        });
    });

    if (overlay) {
        overlay.addEventListener('click', function(event) {
            if (event.target === overlay) {
                closeExpandedPlot();
            }
        });
    }

    document.addEventListener('keydown', function(event) {
        if (event.key === 'Escape') {
            closeExpandedPlot();
        }
    });
});"""

minfy_css = """:root{--page-bg:#edf3f8;--panel-bg:#ffffff;--panel-border:#d7e3ee;--text-main:#16324a;--text-muted:#627486;--accent:#145f80;--accent-soft:#e9f4f9;--shadow:0 10px 26px rgba(17,44,68,.08)}*{box-sizing:border-box}html,body{margin:0;padding:0;background:radial-gradient(circle at top,#f9fcff 0%,var(--page-bg) 42%,#e7eef5 100%);color:var(--text-main);font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif}body{padding:22px}.modal-open{overflow:hidden}.page{max-width:1640px;margin:0 auto}.topbar{display:flex;justify-content:space-between;align-items:flex-start;gap:16px;margin-bottom:18px;flex-wrap:wrap}.hero{background:var(--panel-bg);border:1px solid var(--panel-border);border-radius:22px;box-shadow:var(--shadow);padding:22px 24px;flex:1 1 420px}.hero h1{margin:0 0 8px;font-size:2rem;letter-spacing:.01em}.hero p{margin:0;color:var(--text-muted);font-size:1.02rem}.info-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:12px;flex:2 1 760px}.info-card{background:#fff;border:1px solid var(--panel-border);border-radius:16px;padding:14px 16px;box-shadow:var(--shadow)}.info-card strong{display:block;font-size:.78rem;text-transform:uppercase;letter-spacing:.05em;color:var(--text-muted);margin-bottom:8px}.plot-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:18px;align-items:start}.plot-shell{position:relative;background:var(--panel-bg);border:1px solid var(--panel-border);border-radius:20px;box-shadow:var(--shadow);overflow:hidden;min-width:0;opacity:0;transform:translateY(10px);animation:fadeUp .2s ease-out forwards}.plot-shell.expanded{width:min(96vw,1680px);height:calc(100vh - 32px);margin:0;border-color:#bdd6e4;box-shadow:0 20px 48px rgba(18,43,67,.18);opacity:1;transform:none;animation:none}.plot-head{display:flex;justify-content:space-between;align-items:center;gap:12px;padding:14px 18px;border-bottom:1px solid #e8eef5;background:linear-gradient(180deg,#fcfeff 0%,#f3f8fb 100%)}.plot-head h4{margin:0;font-size:1.04rem;color:var(--accent);word-break:break-word}.plot-tools{display:flex;align-items:center;gap:8px;flex-shrink:0}.plot-expand-btn{appearance:none;border:1px solid #c8dce8;background:var(--accent-soft);color:var(--accent);border-radius:999px;padding:7px 12px;font-size:.86rem;font-weight:700;cursor:pointer;transition:background .12s ease,border-color .12s ease}.plot-expand-btn:hover{background:#dceef7;border-color:#b4d1e0}.plot-body{padding:12px 12px 8px;min-height:420px}.plot-shell.expanded .plot-body{height:calc(100% - 62px);min-height:0}.plot-body>div{width:100%;height:100%}.plot-body .js-plotly-plot{width:100%!important}.js-plotly-plot .plotly .modebar{right:10px!important;top:10px!important}.plot-placeholder{display:none}.plot-overlay{position:fixed;inset:0;background:rgba(16,34,50,.18);opacity:0;visibility:hidden;pointer-events:none;transition:opacity .12s ease;z-index:1000}.plot-overlay.visible{opacity:1;visibility:visible;pointer-events:auto}.plot-overlay-content{width:100%;height:100%;display:flex;align-items:center;justify-content:center;padding:16px}.page-nav{display:flex;justify-content:space-between;align-items:center;gap:12px;margin:18px 0 8px;flex-wrap:wrap}.page-nav a{color:var(--accent);text-decoration:none;font-weight:600;background:var(--accent-soft);border:1px solid #cfe0e8;border-radius:999px;padding:8px 14px;transition:background .16s ease,color .16s ease}.page-nav a:hover{background:#dcebf2}.page-nav .page-count{color:var(--text-muted);font-size:.95rem}.empty-state{background:var(--panel-bg);border:1px dashed var(--panel-border);border-radius:18px;padding:28px;text-align:center;color:var(--text-muted)}footer{text-align:center;color:#54708a;margin:22px 0 6px;font-size:.95rem}@keyframes fadeUp{from{opacity:0;transform:translateY(10px)}to{opacity:1;transform:translateY(0)}}@media (max-width:980px){body{padding:14px}.plot-grid{grid-template-columns:1fr}.plot-head{align-items:flex-start;flex-direction:column}.plot-shell.expanded{width:calc(100vw - 16px);height:calc(100vh - 16px)}.plot-overlay-content{padding:8px}}"""

html_template = f"""<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script src="https://cdn.plot.ly/plotly-3.0.1.min.js"></script>
    <script>{minfy_js}</script>
    <style>{minfy_css}</style>
</head>
<body>
    <div class="page">
        <div class="topbar">
            <section class="hero">
                <h1>Interactive Plots</h1>
                <p>{{{{SENSOR_POSITION}}}} · {{{{INPUT_FILENAME}}}}</p>
            </section>
            <section class="info-grid">
                <div class="info-card"><strong>Tool run on</strong><span>win32</span></div>
                <div class="info-card"><strong>Tool Version</strong><span>1.0</span></div>
                <div class="info-card"><strong>Generated</strong><span>{{{{GENERATION_TIME}}}}</span></div>
                <div class="info-card"><strong>Output File</strong><span>{{{{OUTPUT_FILENAME}}}}</span></div>
            </section>
        </div>
        {{{{TABS}}}}
        {{{{CONTENT}}}}
        <footer>APTIV Interactive Plot Report</footer>
    </div>
    <div id="plot-overlay" class="plot-overlay" aria-hidden="true">
        <div id="plot-overlay-content" class="plot-overlay-content"></div>
    </div>
</body>
</html>"""