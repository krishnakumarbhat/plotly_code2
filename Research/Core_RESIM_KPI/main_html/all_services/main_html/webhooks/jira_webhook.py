"""Flask blueprint to accept Jira webhooks and forward to runtime_store or log.

Register this blueprint in your Flask app as:
    from main_html.webhooks.jira_webhook import bp as jira_webhook_bp
    app.register_blueprint(jira_webhook_bp, url_prefix="/webhook")
"""
from flask import Blueprint, request, jsonify
import logging

bp = Blueprint("jira_webhook", __name__)
_log = logging.getLogger(__name__)


@bp.route("/jira", methods=("POST",))
def jira_webhook():
    payload = request.get_json(silent=True)
    if payload is None:
        return jsonify({"error": "invalid json"}), 400

    # Try to hand off to runtime_store if present
    try:
        from main_html import runtime_store

        if hasattr(runtime_store, "handle_external_event"):
            runtime_store.handle_external_event("jira", payload)
            return jsonify({"status": "accepted"}), 202
    except Exception:
        # runtime_store not available or handler failed; fall back to logging
        _log.info("Jira webhook (fallback log): %s", payload)

    return jsonify({"status": "logged"}), 200
