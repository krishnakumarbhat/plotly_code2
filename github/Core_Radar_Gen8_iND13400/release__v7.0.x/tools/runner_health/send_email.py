"""
send_email.py — Lightweight email dispatcher for Runner Health Reports.

Uses Python's built-in smtplib (zero extra dependencies).
Designed for Aptiv's internal unauthenticated SMTP relay (bulkmail.aptiv.com:25).

Configuration via environment variables:
  SMTP_SERVER      SMTP host.                Default: bulkmail.aptiv.com
  SMTP_PORT        SMTP port.                Default: 25
  EMAIL_FROM       Sender address.           Default: DevSecOps <no_reply@aptiv.com>
  EMAIL_TO         Comma-separated recipients. Required.
  EMAIL_SUBJECT    Subject line.             Default: Runner Health Report — <date>
  EMAIL_BODY_FILE  Path to email_body.html.  Default: email_body.html
  PAGES_URL        Dashboard URL appended to subject when set.
  GITHUB_REPOSITORY  Used in subject/metadata.
  DAYS_BACK        Look-back window label.   Default: 2
"""

import os
import smtplib
import sys
from datetime import datetime
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from zoneinfo import ZoneInfo

# ===== Config =====
SMTP_SERVER = os.getenv("SMTP_SERVER", "bulkmail.aptiv.com")
SMTP_PORT = int(os.getenv("SMTP_PORT", "25"))
EMAIL_FROM = os.getenv("EMAIL_FROM", "DevSecOps <no_reply@aptiv.com>")
EMAIL_TO_RAW = os.getenv("EMAIL_TO", "")
BODY_FILE = os.getenv("EMAIL_BODY_FILE", "email_body.html")
PAGES_URL = os.getenv("PAGES_URL", "")
REPO = os.getenv("GITHUB_REPOSITORY", "")
DAYS_BACK = os.getenv("DAYS_BACK", "2")
TIMEZONE_LABEL = os.getenv("TIMEZONE", "America/Indiana/Indianapolis")

_tz = ZoneInfo(TIMEZONE_LABEL)
_now = datetime.now(_tz).strftime("%Y-%m-%d")

_default_subject = f"[Runner Health] Daily Report — {_now}"
if REPO:
    _default_subject = f"[Runner Health] {REPO} — {_now} (last {DAYS_BACK}d)"
if PAGES_URL:
    _default_subject += f" — {PAGES_URL}"

EMAIL_SUBJECT = os.getenv("EMAIL_SUBJECT", _default_subject)
# =================


def _parse_recipients(raw: str) -> list[str]:
    return [addr.strip() for addr in raw.split(",") if addr.strip()]


def send(recipients: list[str], subject: str, html_body: str) -> None:
    """Send an HTML email with a plain-text fallback via the configured SMTP relay."""
    msg = MIMEMultipart("alternative")
    msg["From"] = EMAIL_FROM
    msg["To"] = ", ".join(recipients)
    msg["Subject"] = subject

    # Plain-text fallback
    plain = (
        f"Runner Health Report — {_now}\n"
        f"Repo: {REPO or 'N/A'} | Period: last {DAYS_BACK} day(s)\n"
        + (f"Dashboard: {PAGES_URL}\n" if PAGES_URL else "")
        + "\nThis report is best viewed in an HTML-capable email client.\n"
    )
    msg.attach(MIMEText(plain, "plain", "utf-8"))
    msg.attach(MIMEText(html_body, "html", "utf-8"))

    print(f"Connecting to {SMTP_SERVER}:{SMTP_PORT} …")
    with smtplib.SMTP(SMTP_SERVER, SMTP_PORT, timeout=30) as smtp:
        smtp.ehlo()
        # Upgrade to TLS if server advertises STARTTLS; skip otherwise (port 25 relay).
        if smtp.has_extn("STARTTLS"):
            smtp.starttls()
            smtp.ehlo()
        smtp.sendmail(EMAIL_FROM, recipients, msg.as_string())
    print(f"✅ Email sent to: {', '.join(recipients)}")


def main() -> None:
    """Validate configuration and dispatch the runner health report email."""
    if not EMAIL_TO_RAW:
        print("ERROR: EMAIL_TO is not set.", file=sys.stderr)
        sys.exit(1)

    recipients = _parse_recipients(EMAIL_TO_RAW)
    if not recipients:
        print("ERROR: EMAIL_TO contains no valid addresses.", file=sys.stderr)
        sys.exit(1)

    if not os.path.isfile(BODY_FILE):
        print(f"ERROR: EMAIL_BODY_FILE not found: {BODY_FILE}", file=sys.stderr)
        sys.exit(1)

    with open(BODY_FILE, encoding="utf-8") as f:
        html_body = f.read()

    print(f"Subject : {EMAIL_SUBJECT}")
    print(f"From    : {EMAIL_FROM}")
    print(f"To      : {', '.join(recipients)}")
    print(f"Body    : {BODY_FILE} ({len(html_body)} bytes)")

    try:
        send(recipients, EMAIL_SUBJECT, html_body)
    except smtplib.SMTPException as exc:
        print(f"ERROR: SMTP failure — {exc}", file=sys.stderr)
        sys.exit(1)
    except OSError as exc:
        print(f"ERROR: Network/OS error — {exc}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
