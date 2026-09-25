"""
send_ci_email.py -- SMTP dispatcher for the Gen8 CI notification email.

Mirrors the dispatch mechanism proven by tools/runner_health/send_email.py:
plain smtplib against Aptiv's internal relay, upgrading to STARTTLS only when
the server advertises it. System.Net.Mail.SmtpClient (the previous PowerShell
path) never negotiates STARTTLS on port 25 and fails with an opaque
"Failure sending mail.", which is why notification emails silently stopped.

Invoked by tools/CI/send_email_notification.ps1, which renders the HTML body.

Configuration via environment variables:
  SMTP_SERVER      SMTP host.                  Default: bulkmail.aptiv.com
  SMTP_PORT        SMTP port.                  Default: 25
  EMAIL_FROM       Sender address.             Default: Gen8 CICD Mail <no_reply@aptiv.com>
  EMAIL_TO         Comma/semicolon recipients. Required.
  EMAIL_SUBJECT    Subject line.               Required.
  EMAIL_BODY_FILE  Path to the HTML body file. Required.
"""

import os
import re
import smtplib
import sys
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

SMTP_SERVER = os.getenv("SMTP_SERVER", "bulkmail.aptiv.com")
SMTP_PORT = int(os.getenv("SMTP_PORT") or "25")
EMAIL_FROM = os.getenv("EMAIL_FROM", "Gen8 CICD Mail <no_reply@aptiv.com>")
EMAIL_TO_RAW = os.getenv("EMAIL_TO", "")
EMAIL_SUBJECT = os.getenv("EMAIL_SUBJECT", "Gen8 CI Notification")
BODY_FILE = os.getenv("EMAIL_BODY_FILE", "")


def parse_recipients(raw: str) -> list:
    """Split a comma- or semicolon-separated recipient list into bare addresses."""
    return [addr.strip() for addr in re.split(r"[;,]", raw) if addr.strip()]


def send(recipients: list, subject: str, html_body: str) -> None:
    """Send an HTML email with a plain-text fallback via the configured SMTP relay."""
    msg = MIMEMultipart("alternative")
    msg["From"] = EMAIL_FROM
    msg["To"] = ", ".join(recipients)
    msg["Subject"] = subject

    plain = f"{subject}\n\nThis report is best viewed in an HTML-capable email client.\n"
    msg.attach(MIMEText(plain, "plain", "utf-8"))
    msg.attach(MIMEText(html_body, "html", "utf-8"))

    print(f"Connecting to {SMTP_SERVER}:{SMTP_PORT} ...")
    with smtplib.SMTP(SMTP_SERVER, SMTP_PORT, timeout=30) as smtp:
        smtp.ehlo()
        # Port 25 relays may or may not offer STARTTLS; only upgrade when advertised.
        if smtp.has_extn("STARTTLS"):
            smtp.starttls()
            smtp.ehlo()
        smtp.sendmail(EMAIL_FROM, recipients, msg.as_string())
    print(f"Email sent to: {', '.join(recipients)}")


def main() -> None:
    """Validate configuration and dispatch the CI notification email."""
    if not EMAIL_TO_RAW:
        print("ERROR: EMAIL_TO is not set.", file=sys.stderr)
        sys.exit(1)

    recipients = parse_recipients(EMAIL_TO_RAW)
    if not recipients:
        print("ERROR: EMAIL_TO contains no valid addresses.", file=sys.stderr)
        sys.exit(1)

    if not BODY_FILE or not os.path.isfile(BODY_FILE):
        print(f"ERROR: EMAIL_BODY_FILE not found: {BODY_FILE!r}", file=sys.stderr)
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
        print(f"ERROR: SMTP failure -- {exc}", file=sys.stderr)
        sys.exit(1)
    except OSError as exc:
        print(f"ERROR: Network/OS error -- {exc}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
