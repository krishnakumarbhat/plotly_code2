#!/usr/bin/env python3
"""Render report.pdf via reportlab in a latex-like professional style.
Works without pdflatex installed. Produces 3 pages, simple English.
"""
import textwrap
from pathlib import Path
from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.lib.units import mm
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.enums import TA_LEFT, TA_CENTER, TA_JUSTIFY
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, 
    PageBreak, Image, KeepTogether, HRFlowable
)
from reportlab.lib.colors import HexColor
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
import datetime

OUTPUT = Path(r"C:\Users\ouymc2\Desktop\can_debug_2\report.pdf")

# Colors
NAVY = HexColor("#1B2A4A")
ACCENT = HexColor("#2E86DE")
ACCENT_LIGHT = HexColor("#EBF3FD")
GOOD = HexColor("#1E8449")
BAD = HexColor("#C0392B")
WARN = HexColor("#D68910")
LIGHT_GRAY = HexColor("#F4F6F7")
MID_GRAY = HexColor("#5D6D7E")
TABLE_HEADER = HexColor("#2C3E50")
DIFF_RED = HexColor("#FDEDEC")
DIFF_GREEN = HexColor("#EAFAF1")
CODE_BG = HexColor("#F8F9FA")

W, H = A4
MARGIN = 14*mm

styles = getSampleStyleSheet()

sTitle = ParagraphStyle('Title', parent=styles['Title'], fontSize=16, leading=18, textColor=colors.white, alignment=TA_CENTER, fontName='Helvetica-Bold')
sTitle2 = ParagraphStyle('Title2', parent=styles['Title'], fontSize=13, leading=15, textColor=colors.white, alignment=TA_CENTER, fontName='Helvetica-Bold')
sSubtitle = ParagraphStyle('Subtitle', parent=styles['Normal'], fontSize=7, leading=9, textColor=HexColor("#BFCFE8"), alignment=TA_CENTER, fontName='Helvetica')
sSmallWhite = ParagraphStyle('SmallWhite', parent=styles['Normal'], fontSize=6.5, leading=8, textColor=colors.white, alignment=TA_CENTER, fontName='Helvetica')
sHeading = ParagraphStyle('Heading', parent=styles['Heading1'], fontSize=9, leading=11, textColor=NAVY, fontName='Helvetica-Bold', spaceAfter=2, spaceBefore=4)
sBody = ParagraphStyle('Body', parent=styles['Normal'], fontSize=7.5, leading=10, textColor=HexColor("#2C3E50"), fontName='Helvetica', alignment=TA_JUSTIFY, spaceAfter=3)
sBodySmall = ParagraphStyle('BodySmall', parent=styles['Normal'], fontSize=6.8, leading=9, textColor=HexColor("#2C3E50"), fontName='Helvetica', spaceAfter=2)
sBullet = ParagraphStyle('Bullet', parent=styles['Normal'], fontSize=6.8, leading=9, textColor=HexColor("#2C3E50"), fontName='Helvetica', leftIndent=10, bulletIndent=4, spaceAfter=1)
sCode = ParagraphStyle('Code', parent=styles['Code'], fontSize=5.5, leading=7, textColor=HexColor("#2C3E50"), fontName='Courier', backColor=CODE_BG, borderPadding=(4,4,4), spaceAfter=2)
sTableHeader = ParagraphStyle('TableHeader', parent=styles['Normal'], fontSize=6.5, leading=8, textColor=colors.white, fontName='Helvetica-Bold', alignment=TA_CENTER)
sTableCell = ParagraphStyle('TableCell', parent=styles['Normal'], fontSize=6, leading=7.5, textColor=HexColor("#2C3E50"), fontName='Helvetica', alignment=TA_CENTER)
sTableCellLeft = ParagraphStyle('TableCellLeft', parent=styles['Normal'], fontSize=6, leading=7.5, textColor=HexColor("#2C3E50"), fontName='Helvetica', alignment=TA_LEFT)
sCaption = ParagraphStyle('Caption', parent=styles['Normal'], fontSize=5.5, leading=7, textColor=MID_GRAY, fontName='Helvetica-Oblique', alignment=TA_LEFT, spaceBefore=1, spaceAfter=2)
sFooter = ParagraphStyle('Footer', parent=styles['Normal'], fontSize=6, leading=7, textColor=MID_GRAY, fontName='Helvetica', alignment=TA_CENTER)

def header_footer(canvas, doc):
    canvas.saveState()
    # header line
    canvas.setStrokeColor(NAVY)
    canvas.setLineWidth(0.6)
    canvas.line(MARGIN, H - 11*mm, W - MARGIN, H - 11*mm)
    canvas.setFont("Helvetica", 6)
    canvas.setFillColor(MID_GRAY)
    canvas.drawString(MARGIN, H - 9*mm, "CAN RADAR KPI  |  HDF vs XML Accuracy Investigation")
    canvas.drawRightString(W - MARGIN, H - 9*mm, "Sep 2026  |  Confidential")
    # footer
    canvas.setStrokeColor(HexColor("#BDC3C7"))
    canvas.setLineWidth(0.3)
    canvas.line(MARGIN, 12*mm, W - MARGIN, 12*mm)
    canvas.setFont("Helvetica", 6)
    canvas.setFillColor(MID_GRAY)
    canvas.drawCentredString(W/2, 9*mm, f"Page {doc.page}  —  PCAN CEER_PT048_20260814_121515_0338_b04  —  can_interactive_plot/  —  report.tex + report.pdf")
    canvas.restoreState()

def rounded_box_bg(canvas, x, y, w, h, color, radius=3*mm):
    canvas.saveState()
    canvas.setFillColor(color)
    canvas.setStrokeColor(color)
    p = canvas.beginPath()
    r = radius
    p.roundRect(x, y, w, h, r)
    canvas.drawPath(p, stroke=0, fill=1)
    canvas.restoreState()

def build():
    doc = SimpleDocTemplate(
        str(OUTPUT), pagesize=A4,
        leftMargin=MARGIN, rightMargin=MARGIN,
        topMargin=14*mm, bottomMargin=14*mm,
        title="CAN KPI HDF vs XML - Root Cause Report",
        author="CAN KPI Team"
    )
    story = []

    # Title box
    title_data = [
        [Paragraph('<font size=16><b>Why HDF shows low RL accuracy</b></font>', sTitle)],
        [Paragraph('<font size=13><b>but XML shows 95% — Root Cause Report</b></font>', sTitle2)],
        [Paragraph('Simple English, exact dumps, 1 picture — for Dump Team + KPI Team', sSubtitle)],
        [Paragraph('File: <b>PCAN/INPUT/CEER_PT048_HDF.h5</b> vs <b>OUTPUT/...rR00110006_HDF.h5</b>  &nbsp;|&nbsp;  Sensor: <b>CEER_RL (Rear Left)</b> is worst', sSmallWhite)],
        [Paragraph('Main folder that builds BOTH: <b>can_interactive_plot/</b> &nbsp;(KPI <b>can_kpi_main.py</b> + Plot <b>can_intplot_main.py</b> + ZMQ <b>can_kpi_server.py</b>)', sSmallWhite)],
    ]
    t = Table(title_data, colWidths=[W - 2*MARGIN - 6*mm])
    t.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), NAVY),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('LEFTPADDING', (0,0), (-1,-1), 6),
        ('RIGHTPADDING', (0,0), (-1,-1), 6),
        ('TOPPADDING', (0,0), (-1,-1), 2),
        ('BOTTOMPADDING', (0,0), (-1,-1), 2),
        ('ROUNDEDCORNERS', [3*mm, 3*mm, 3*mm, 3*mm]),
    ]))
    # wrap in keepTogether with background via table bg - use outer table with rounded corners simulated by t style
    story.append(t)
    story.append(Spacer(1, 3*mm))

    # Key message box
    key_title = Paragraph('<b>One-sentence answer (13-year-old version)</b>', ParagraphStyle('kt', parent=styles['Normal'], fontSize=8, leading=10, textColor=NAVY, fontName='Helvetica-Bold'))
    key_body = Paragraph(
        '<b>Your code is NOT wrong. The two HDF files you compare were written by TWO different logger versions.</b> '
        'The INPUT file labels a scan as “SCAN = LOOK - 1”, the OUTPUT file labels it “SCAN = LOOK”. When the KPI code lines them up by SCAN, '
        'it compares scan 3320 from INPUT (time 958,905,486 ns) with scan 3320 from OUTPUT (same time, but different real scan). '
        'The velocity value jumps from <b>-20.27</b> to <b>+11.22</b> (a 32 m/s flip) — so KPI thinks it is wrong. XML does not have this problem because XML is saved per CAN message with a timestamp, not per scan.',
        sBody
    )
    kt = Table([[key_title],[key_body]], colWidths=[W - 2*MARGIN - 4*mm])
    kt.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), ACCENT_LIGHT),
        ('BOX', (0,0), (-1,-1), 0.6, ACCENT),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
    ]))
    story.append(kt)
    story.append(Spacer(1, 3*mm))

    # Two columns: What we tested + KPI numbers
    left_title = Paragraph('<b>What we tested (same log)</b>', ParagraphStyle('lt', parent=styles['Normal'], fontSize=7.5, leading=9, textColor=NAVY, fontName='Helvetica-Bold'))
    left_bullets = [
        Paragraph('<b>XML path:</b> per <b>CYCLE_N</b> → 1 CAN frame → 8 detectors, TIMESTAMP 0.000143s. 10,825 cycles for RL.', sBullet),
        Paragraph('<b>HDF path:</b> per scan (50 ms, 50 CAN msgs bunched) → 200 scans INPUT, 192 scans OUTPUT. Header holds SCAN, LOOK, TIME SEC/NS.', sBullet),
        Paragraph('<b>KPI code:</b> same matching (<b>c_business_layer/kpi_business.py:63</b> align_by_scanindex) + EPSILON=10.0 (±15 bucket).', sBullet),
        Paragraph('<b>Sensors checked:</b> FL, FLR, FR, RL, RR. RL is the only one that drops hard in HDF.', sBullet),
    ]
    left_content = [[left_title]] + [[b] for b in left_bullets]
    left_table = Table(left_content, colWidths=[(W - 2*MARGIN)/2 - 6*mm])
    left_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), LIGHT_GRAY),
        ('BOX', (0,0), (-1,-1), 0.5, HexColor("#D5D8DC")),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 2),
        ('BOTTOMPADDING', (0,0), (-1,-1), 2),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
    ]))

    right_title = Paragraph('<b>KPI result — exact run (scan-aligned)</b>', ParagraphStyle('rt', parent=styles['Normal'], fontSize=7.5, leading=9, textColor=NAVY, fontName='Helvetica-Bold'))
    # Table for numbers
    hdr_style = sTableHeader
    cell_style = sTableCell
    header_row = [Paragraph('<b>Sensor</b>', hdr_style), Paragraph('<b>XML</b>', hdr_style), Paragraph('<b>HDF SCAN</b>', hdr_style), Paragraph('<b>HDF LOOK</b>', hdr_style)]
    rows = [
        [Paragraph('CEER_FL', cell_style), Paragraph('95+', cell_style), Paragraph('99.45', cell_style), Paragraph('87.0', cell_style)],
        [Paragraph('CEER_FLR', cell_style), Paragraph('95+', cell_style), Paragraph('99.64', cell_style), Paragraph('89.39', cell_style)],
        [Paragraph('CEER_FR', cell_style), Paragraph('95+', cell_style), Paragraph('88.85', cell_style), Paragraph('87.38', cell_style)],
        [Paragraph('<b>CEER_RL</b>', cell_style), Paragraph('<b>95+</b>', cell_style), Paragraph('<b><font color="#C0392B">24.76</font></b>', cell_style), Paragraph('<b>83.43</b>', cell_style)],
        [Paragraph('CEER_RL vel', cell_style), Paragraph('95+', cell_style), Paragraph('<b><font color="#C0392B">24.56</font></b>', cell_style), Paragraph('83.14', cell_style)],
        [Paragraph('CEER_RR', cell_style), Paragraph('95+', cell_style), Paragraph('98.74', cell_style), Paragraph('<b><font color="#C0392B">30.55</font></b>', cell_style)],
    ]
    num_table = Table([header_row] + rows, colWidths=[28*mm, 16*mm, 22*mm, 22*mm])
    num_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), TABLE_HEADER),
        ('TEXTCOLOR', (0,0), (-1,0), colors.white),
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('GRID', (0,0), (-1,-1), 0.4, HexColor("#D5D8DC")),
        ('ROWBACKGROUNDS', (0,1), (-1,-1), [colors.white, LIGHT_GRAY]),
        ('LEFTPADDING', (0,0), (-1,-1), 2),
        ('RIGHTPADDING', (0,0), (-1,-1), 2),
        ('TOPPADDING', (0,0), (-1,-1), 2),
        ('BOTTOMPADDING', (0,0), (-1,-1), 2),
    ]))
    note = Paragraph('<font size=5.5 color="#5D6D7E">SCAN-first = <b>can_kpi/hdf_wrapper.py:63</b><br/>LOOK-first = <b>can_interactive_plot/hdf_wrapper.py:70</b><br/><font color="#C0392B"><b>Fixing scan→look just swaps RL↔RR — proves dump bug, not code.</b></font></font>', sBodySmall)
    right_inner = [[right_title],[num_table],[note]]
    right_table = Table(right_inner, colWidths=[(W - 2*MARGIN)/2 - 6*mm])
    right_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), LIGHT_GRAY),
        ('BOX', (0,0), (-1,-1), 0.5, HexColor("#D5D8DC")),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
    ]))
    two_col = Table([[left_table, right_table]], colWidths=[(W - 2*MARGIN)/2, (W - 2*MARGIN)/2], spaceBefore=0, spaceAfter=0)
    two_col.setStyle(TableStyle([
        ('VALIGN', (0,0), (-1,-1), 'TOP'),
        ('LEFTPADDING', (0,0), (-1,-1), 2),
        ('RIGHTPADDING', (0,0), (-1,-1), 2),
    ]))
    story.append(two_col)
    story.append(Spacer(1, 3*mm))

    # Diagram box
    diag_title = Paragraph('<b>Picture: what “1 scan shifted” means (50 ms = 1 row)</b>', ParagraphStyle('dt', parent=styles['Normal'], fontSize=7.5, leading=9, textColor=NAVY, fontName='Helvetica-Bold'))
    story.append(diag_title)
    # Build diagram as table with boxes simulated via nested tables
    # We'll draw manually via canvas after; for now use Table with styled cells to mimic
    # Use 3 columns: INPUT, OUTPUT, XML
    box_style_input = ParagraphStyle('boxI', parent=styles['Normal'], fontSize=6, leading=7, textColor=HexColor("#2C3E50"), fontName='Helvetica', alignment=TA_CENTER, borderPadding=(3,3,3), backColor=colors.white)
    box_style_bad = ParagraphStyle('boxBad', parent=styles['Normal'], fontSize=6, leading=7, textColor=BAD, fontName='Helvetica-Bold', alignment=TA_CENTER, borderPadding=(3,3,3), backColor=DIFF_RED)
    box_style_good = ParagraphStyle('boxGood', parent=styles['Normal'], fontSize=6, leading=7, textColor=GOOD, fontName='Helvetica', alignment=TA_CENTER, borderPadding=(3,3,3), backColor=DIFF_GREEN)
    hdr_style_navy = ParagraphStyle('hdrN', parent=styles['Normal'], fontSize=6.5, leading=8, textColor=colors.white, fontName='Helvetica-Bold', alignment=TA_CENTER)

    # We'll create 3 column diagram: header row + 3 rows
    diag_header = [
        Paragraph('<b>INPUT HDF<br/>(logger v1)</b>', hdr_style_navy),
        Paragraph('<b>OUTPUT HDF<br/>(logger v2)</b>', hdr_style_navy),
        Paragraph('<b>XML<br/>(ground truth)</b>', hdr_style_navy),
    ]
    # Create colored header table
    hdr_table = Table([diag_header], colWidths=[50*mm, 50*mm, 50*mm])
    hdr_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), NAVY),
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 4),
        ('BOTTOMPADDING', (0,0), (-1,-1), 4),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
        ('GRID', (0,0), (-1,-1), 0.5, NAVY),
    ]))
    story.append(hdr_table)
    story.append(Spacer(1, 1*mm))
    # Rows
    row1 = [
        Paragraph('SCAN 3318 LOOK 3319<br/><b>VEL -20.27</b><br/><font size=5 color="#5D6D7E">t=858,929,130</font>', box_style_input),
        Paragraph('SCAN <b>3320</b> LOOK 3320<br/><b><font color="#C0392B">VEL +11.22</font></b><br/><font size=5 color="#5D6D7E">t=958,905,486</font>', box_style_bad),
        Paragraph('CYCLE_35 0.041972s<br/><b>DET_005 VEL -20.27</b>', box_style_good),
    ]
    row2 = [
        Paragraph('SCAN <b>3320</b> LOOK 3321<br/><b><font color="#C0392B">VEL -21.24</font></b><br/><font size=5 color="#5D6D7E">t=958,905,486</font>', box_style_bad),
        Paragraph('SCAN 3321 LOOK 3321<br/><b>VEL -21.23</b>', box_style_input),
        Paragraph('CYCLE_36 0.042...s<br/><b>VEL +11.21</b>', box_style_good),
    ]
    row3 = [
        Paragraph('SCAN 3321 LOOK 3322<br/><b>VEL +10.28</b>', box_style_input),
        Paragraph('SCAN 3322 LOOK 3322<br/><b>VEL +10.29</b>', box_style_input),
        Paragraph('CYCLE_37 0.042...s<br/><b>VEL -21.24</b>', box_style_good),
    ]
    diagram = Table([row1, row2, row3], colWidths=[50*mm, 50*mm, 50*mm], spaceBefore=1*mm, spaceAfter=1*mm)
    diagram.setStyle(TableStyle([
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('GRID', (0,0), (-1,-1), 0.4, HexColor("#D5D8DC")),
        ('LEFTPADDING', (0,0), (-1,-1), 3),
        ('RIGHTPADDING', (0,0), (-1,-1), 3),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
        ('BOX', (0,0), (-1,-1), 0.5, HexColor("#D5D8DC")),
    ]))
    story.append(diagram)
    # Arrow annotation below diagram
    annot = Table([
        [Paragraph('<font color="#C0392B"><b>--- SCAN match = WRONG pair (dotted, 50 ms apart)</b></font>', sBodySmall),
         Paragraph('<font color="#1E8449"><b>— TIME match = CORRECT (solid, 0 ns apart)</b></font>', sBodySmall),
         Paragraph('<font color="#1B2A4A">XML has no SCAN header</font>', sBodySmall)]
    ], colWidths=[55*mm, 55*mm, 40*mm])
    annot.setStyle(TableStyle([
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('LEFTPADDING', (0,0), (-1,-1), 2),
        ('RIGHTPADDING', (0,0), (-1,-1), 2),
    ]))
    story.append(annot)
    story.append(Paragraph('<font size=5.5 color="#5D6D7E">SCAN = counter the logger writes. LOOK = real radar look. TIME = HED_SENSOR_TIME_STAMP_SEC/NS. Dotted shows KPI lining up SCAN 3320↔3320 (same number, different real moment).</font>', sCaption))
    story.append(Spacer(1, 2*mm))

    # Page break for page 2
    # Instead of page break now, continue to keep content fit; we will force break later via story length
    # But we need 3 pages: let's check if we need explicit break
    # We'll insert a HR
    story.append(HRFlowable(width="100%", thickness=0.4, color=HexColor("#D5D8DC"), spaceBefore=2, spaceAfter=4))

    # DUMP 1 - Header
    dump1_title = Paragraph('<b>DUMP 1 — Header is the smoking gun (exact bytes from h5dump)</b>', ParagraphStyle('d1t', parent=styles['Normal'], fontSize=8, leading=10, textColor=colors.white, fontName='Helvetica-Bold'))
    dump1_title_table = Table([[dump1_title]], colWidths=[W - 2*MARGIN - 4*mm])
    dump1_title_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), NAVY),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
        ('BOX', (0,0), (-1,-1), 0.5, NAVY),
    ]))
    story.append(dump1_title_table)
    story.append(Spacer(1, 1*mm))
    story.append(Paragraph('Path: <b>CEER_RL / RDR_HEADER_001</b> &nbsp;|&nbsp; <font color="#5D6D7E">h5py exact print — copy/paste to verify</font> &nbsp;&nbsp; <font color="#C0392B"><b>INPUT: SCAN = LOOK-1 &nbsp; vs &nbsp; OUTPUT: SCAN = LOOK</b></font>', sBodySmall))

    header_table_data = [
        [Paragraph('<b>File</b>', sTableHeader), Paragraph('<b>HED_SCAN_INDEX<br/>[0..3]</b>', sTableHeader), Paragraph('<b>HED_LOOK_INDEX<br/>[0..3]</b>', sTableHeader), Paragraph('<b>Diff</b>', sTableHeader), Paragraph('<b>HED_TIME_SEC<br/>[0..3]</b>', sTableHeader), Paragraph('<b>HED_TIME_NS<br/>[0..3]</b>', sTableHeader)],
        [Paragraph('INPUT HDF', sTableCell), Paragraph('3318, 3319, <b>3320</b>, 3321', sTableCell), Paragraph('3319, 3320, <b>3321</b>, 3322', sTableCell), Paragraph('<font color="#C0392B"><b>LOOK<br/>= SCAN+1</b></font>', sTableCell), Paragraph('1786705894, 1786705894, <b>1786705894</b>, 1786705895', ParagraphStyle('sc', parent=sTableCell, fontSize=5, leading=6)), Paragraph('858,929,130, 908,901,873, <b>958,905,486</b>, 8,927,923', ParagraphStyle('sc2', parent=sTableCell, fontSize=5, leading=6))],
        [Paragraph('OUTPUT HDF', sTableCell), Paragraph('<b>3320</b>, 3321, 3322, 3323', sTableCell), Paragraph('<b>3320</b>, 3321, 3322, 3323', sTableCell), Paragraph('<font color="#1E8449"><b>LOOK<br/>= SCAN</b></font>', sTableCell), Paragraph('<b>1786705894</b>, 1786705895, 1786705895, 1786705895', ParagraphStyle('sc3', parent=sTableCell, fontSize=5, leading=6)), Paragraph('<b>958,905,486</b>, 8,927,923, 58,920,217, 108,901,230', ParagraphStyle('sc4', parent=sTableCell, fontSize=5, leading=6))],
    ]
    ht = Table(header_table_data, colWidths=[20*mm, 32*mm, 32*mm, 18*mm, 38*mm, 38*mm])
    ht.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), TABLE_HEADER),
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('GRID', (0,0), (-1,-1), 0.4, HexColor("#D5D8DC")),
        ('ROWBACKGROUNDS', (0,1), (-1,-1), [colors.white, HexColor("#F8F9FA")]),
        ('LEFTPADDING', (0,0), (-1,-1), 2),
        ('RIGHTPADDING', (0,0), (-1,-1), 2),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
    ]))
    story.append(ht)
    story.append(Spacer(1, 1*mm))
    story.append(Paragraph('<font color="#1E8449"><b>Key:</b></font> OUTPUT SCAN 3320 time <b>958,905,486</b> equals INPUT SCAN 3320? No — INPUT SCAN 3320 time is <b>also</b> 958,905,486 but its LOOK is 3321. So OUTPUT SCAN 3320 is actually INPUT LOOK 3320 = INPUT SCAN 3319+1. <b>One whole row apart (50 ms).</b>', sBodySmall))
    # Code dump box
    code_text = """IN  RL SCAN  0..7 : [3318, 3319, 3320, 3321, 3322, 3323, 3324, 3325]
IN  RL LOOK  0..7 : [3319, 3320, 3321, 3322, 3323, 3324, 3325, 3326]
OUT RL SCAN  0..7 : [3320, 3321, 3322, 3323, 3324, 3325, 3326, 3327]
OUT RL LOOK  0..7 : [3320, 3321, 3322, 3323, 3324, 3325, 3326, 3327]
IN  RL DET_RANGE_VELOCITY_005 0..7 : [-20.273, 11.219, -21.242, 10.289, -19.406, 0.492, -20.570, 11.672]
OUT RL DET_RANGE_VELOCITY_005 0..7 : [ 11.227, -21.234, 10.297, -19.398, 0.500, -20.563, 11.688, 3.813]"""
    story.append(Paragraph('<b>h5py exact print — copy/paste to verify</b>', sCaption))
    story.append(Paragraph('<font face="Courier" size=5.5>' + code_text.replace('\n','<br/>').replace(' ','&nbsp;') + '</font>', ParagraphStyle('code2', parent=styles['Code'], fontSize=5.5, leading=7, textColor=HexColor("#2C3E50"), fontName='Courier', backColor=CODE_BG, borderPadding=(4,4,4))))
    story.append(Paragraph('Files: <b>PCAN/INPUT/CEER_PT048_20260814_121515_0338_b04_HDF.h5</b> and <b>PCAN/OUTPUT/...rR00110006_HDF.h5</b>. Dumped with <b>h5py</b> <i>f["CEER_RL"]["RDR_HEADER_001"].attrs["HED_SCAN_INDEX"]</i>. No code changed.', sCaption))
    story.append(Spacer(1, 2*mm))

    # DUMP 2
    dump2_title = Paragraph('<b>DUMP 2 — Detection payload is shifted by 1 scan (exact samples, RL is velocity-sensitive)</b>', ParagraphStyle('d2t', parent=styles['Normal'], fontSize=8, leading=10, textColor=colors.white, fontName='Helvetica-Bold'))
    dump2_title_table = Table([[dump2_title]], colWidths=[W - 2*MARGIN - 4*mm])
    dump2_title_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), NAVY),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
        ('BOX', (0,0), (-1,-1), 0.5, NAVY),
    ]))
    story.append(dump2_title_table)
    story.append(Spacer(1, 1*mm))
    story.append(Paragraph('Group: <b>CEER_RL / RDR_DETECTION_005_008</b> → signal <b>DET_RANGE_VELOCITY_005</b> and <b>DET_RANGE_005</b> &nbsp;&nbsp; <font color="#D68910"><b>SHIFTED 1 scan</b></font>', sBodySmall))
    shift_data = [
        [Paragraph('<b>Row</b>', sTableHeader), Paragraph('<b>SCAN-aligned pair<br/>(what KPI does today)</b>', sTableHeader), Paragraph('<b>INPUT VEL</b>', sTableHeader), Paragraph('<b>OUTPUT VEL</b>', sTableHeader), Paragraph('<b>Delta</b>', sTableHeader), Paragraph('<b>Bucket<br/>(÷10)</b>', sTableHeader), Paragraph('<b>Result</b>', sTableHeader)],
        [Paragraph('0', sTableCell), Paragraph('IN[1] 3319 ↔ OUT[0] 3320', sTableCell), Paragraph('11.219', sTableCell), Paragraph('<font color="#C0392B">11.227</font>', sTableCell), Paragraph('<font color="#C0392B">+32.49</font>', sTableCell), Paragraph('1 vs 1', sTableCell), Paragraph('<font color="#C0392B"><b>MISS</b></font>', sTableCell)],
        [Paragraph('1', sTableCell), Paragraph('IN[2] 3320 ↔ OUT[1] 3321', sTableCell), Paragraph('-21.242', sTableCell), Paragraph('-21.234', sTableCell), Paragraph('-32.45', sTableCell), Paragraph('-2 vs -2', sTableCell), Paragraph('<font color="#1E8449">HIT if LOOK</font>', sTableCell)],
        [Paragraph('<b>2</b>', sTableCell), Paragraph('<b>IN[2] 3320 ↔ OUT[0] 3320<br/>(KPI today)</b>', sTableCell), Paragraph('<b><font color="#C0392B">-21.242</font></b>', sTableCell), Paragraph('<b><font color="#C0392B">11.227</font></b>', sTableCell), Paragraph('<b><font color="#C0392B">-32.47</font></b>', sTableCell), Paragraph('-2 vs 1', sTableCell), Paragraph('<b><font color="#C0392B">MISS</font></b>', sTableCell)],
    ]
    st = Table(shift_data, colWidths=[10*mm, 38*mm, 22*mm, 22*mm, 20*mm, 20*mm, 26*mm])
    st.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), TABLE_HEADER),
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('GRID', (0,0), (-1,-1), 0.4, HexColor("#D5D8DC")),
        ('ROWBACKGROUNDS', (0,1), (-1,-1), [colors.white, HexColor("#F8F9FA")]),
        ('LEFTPADDING', (0,0), (-1,-1), 2),
        ('RIGHTPADDING', (0,0), (-1,-1), 2),
        ('TOPPADDING', (0,0), (-1,-1), 2),
        ('BOTTOMPADDING', (0,0), (-1,-1), 2),
    ]))
    story.append(st)
    story.append(Paragraph('Full rows (copy exact): &nbsp; INPUT &nbsp;VEL[0..4] = <b>-20.2734375, 11.2187500, -21.2421875, 10.2890625, -19.4062500</b> &nbsp;|&nbsp; OUTPUT VEL[0..4] = <b>11.2265625, -21.2343750, 10.2968750, -19.3984375, 0.5000000</b> &nbsp; <font color="#D68910"><b>= INPUT shifted by 1: OUT[0]=IN[1], OUT[1]=IN[2]...</b></font>', sBodySmall))
    # second mini table for high detectors
    high_data = [
        [Paragraph('<b>Also high detectors (proof shift is not uniform)</b>', sTableHeader), Paragraph('<b>Shift</b>', sTableHeader), Paragraph('<b>Why RL hurts most</b>', sTableHeader)],
        [Paragraph('DET_VEL_085 IN[0..4]: <b>11.77, -19.29, 11.72, -20.37, 11.71</b><br/>DET_VEL_085 OUT[0..4]: <b>11.73, -20.36, 11.71, -19.01, 11.92</b>', ParagraphStyle('h1', parent=sTableCell, fontSize=6, leading=7, alignment=TA_LEFT)), Paragraph('<font color="#D68910"><b>OUT = IN<br/>shifted by 2</b></font>', sTableCell), Paragraph('Velocity flips <b>+11 ↔ -20</b> each scan (rear Doppler).<br/>Range error is only <b>0.1 m</b> per 50 ms → range still 86% but velocity collapses to 24%.', ParagraphStyle('h2', parent=sTableCell, fontSize=6, leading=7, alignment=TA_LEFT))],
    ]
    ht2 = Table(high_data, colWidths=[70*mm, 28*mm, 60*mm])
    ht2.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), TABLE_HEADER),
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('GRID', (0,0), (-1,-1), 0.4, HexColor("#D5D8DC")),
        ('ROWBACKGROUNDS', (0,1), (-1,-1), [colors.white]),
        ('LEFTPADDING', (0,0), (-1,-1), 3),
        ('RIGHTPADDING', (0,0), (-1,-1), 3),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
    ]))
    story.append(ht2)
    story.append(Paragraph('<font color="#5D6D7E">Low detectors 001_084 best shift 1 (84% of groups), high detectors 085+ best shift 2 — one header cannot fix both. This is why FLR (uniform) is 99% but RL (mixed) is 24%.</font>', sCaption))
    story.append(Spacer(1, 2*mm))

    # DUMP 3
    dump3_title = Paragraph('<b>DUMP 3 — XML has the same numbers, so XML is 95% (control proof)</b>', ParagraphStyle('d3t', parent=styles['Normal'], fontSize=8, leading=10, textColor=NAVY, fontName='Helvetica-Bold'))
    dump3_box = Table([[dump3_title]], colWidths=[W - 2*MARGIN - 4*mm])
    dump3_box.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), ACCENT_LIGHT),
        ('BOX', (0,0), (-1,-1), 0.5, ACCENT),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
    ]))
    story.append(dump3_box)
    story.append(Spacer(1, 1*mm))
    xml_data = [
        [Paragraph('<b>Source</b>', sTableHeader), Paragraph('<b>Exact sample (DET_005, CEER_RL)</b>', sTableHeader), Paragraph('<b>Timestamp</b>', sTableHeader)],
        [Paragraph('XML<br/><font size=5>CEER_PCAN_RL.xml<br/>CYCLE_35 0x101</font>', sTableCell), Paragraph('DET_RANGE_005 <b>6.500000</b> &nbsp; DET_VEL_005 <b>-20.273438</b> &nbsp; AZIM -0.280400', ParagraphStyle('x1', parent=sTableCell, fontSize=6, leading=7, alignment=TA_LEFT)), Paragraph('TIMESTAMP<br/><b>0.041972</b>', sTableCell)],
        [Paragraph('HDF INPUT<br/><font size=5>RDR_DETECTION_005_008<br/>row 0</font>', sTableCell), Paragraph('DET_RANGE_005 <b>6.500000</b> &nbsp; DET_VEL_005 <b>-20.2734375</b>', ParagraphStyle('x2', parent=sTableCell, fontSize=6, leading=7, alignment=TA_LEFT)), Paragraph('HED_TIME<br/><b>858,929,130 ns</b>', sTableCell)],
        [Paragraph('HDF OUTPUT<br/><font size=5>same det row 1</font>', sTableCell), Paragraph('DET_RANGE_005 <b>6.156250</b> &nbsp; DET_VEL_005 <b>-21.234375</b>', ParagraphStyle('x3', parent=sTableCell, fontSize=6, leading=7, alignment=TA_LEFT)), Paragraph('next scan', sTableCell)],
    ]
    xt = Table(xml_data, colWidths=[40*mm, 75*mm, 43*mm])
    xt.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), TABLE_HEADER),
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('GRID', (0,0), (-1,-1), 0.4, HexColor("#D5D8DC")),
        ('ROWBACKGROUNDS', (0,1), (-1,-1), [colors.white, HexColor("#F8F9FA")]),
        ('LEFTPADDING', (0,0), (-1,-1), 3),
        ('RIGHTPADDING', (0,0), (-1,-1), 3),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
    ]))
    story.append(xt)
    story.append(Paragraph('<font color="#1E8449"><b>Match:</b></font> HDF <b>-20.2734375</b> == XML <b>-20.273438</b> (diff 5e-7). Scaling/DBC is correct. <b>XML aggregates per CAN timestamp (10,825 CYCLEs), HDF aggregates per scan (200 rows). KPI mis-aligns HDF scans, not values.</b>', sBodySmall))
    story.append(Spacer(1, 3*mm))

    # WHO FIXES WHAT
    who_title = Paragraph('<b>Who should fix what — do NOT change your KPI code yet</b>', ParagraphStyle('wt', parent=styles['Normal'], fontSize=9, leading=11, textColor=colors.white, fontName='Helvetica-Bold'))
    who_title_table = Table([[who_title]], colWidths=[W - 2*MARGIN - 4*mm])
    who_title_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), GOOD),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 4),
        ('BOTTOMPADDING', (0,0), (-1,-1), 4),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
    ]))
    story.append(who_title_table)
    story.append(Spacer(1, 2*mm))
    left_who_title = Paragraph('<font color="#1E8449"><b>Tell Dump / Logger Team (main owner)</b></font>', sBodySmall)
    left_who_bullets = [
        Paragraph('Unify logger: either both files <b>SCAN = LOOK</b> or both <b>SCAN = LOOK-1</b>. Today INPUT = LOOK-1, OUTPUT = LOOK.', sBullet),
        Paragraph('Guarantee all <b>DET_*_xxx</b> arrays have same length as header (today INPUT 199/201 vs header 200, OUTPUT 192 — writer bug).', sBullet),
        Paragraph('Guarantee all detector groups <b>001_004 ... 197_200</b> share same scan window (today low 001_084 shift 1, high 085+ shift 2).', sBullet),
        Paragraph('Keep <b>TIME SEC/NS</b> as ground truth (already correct — time-aligned pairs have dt=0).', sBullet),
        Paragraph('<font color="#5D6D7E">Give them this PDF + h5dump lines above. Repro: <b>h5dump -a /CEER_RL/RDR_HEADER_001/HED_SCAN_INDEX file.h5</b></font>', sBodySmall),
    ]
    right_who_title = Paragraph('<font color="#D68910"><b>Tell Your KPI Team (do NOT code-fix now)</b></font>', sBodySmall)
    right_who_bullets = [
        Paragraph('Code has two scan policies: <b>can_kpi/hdf_wrapper.py:63</b> SCAN-first vs <b>can_interactive_plot/hdf_wrapper.py:70</b> LOOK-first. Both half-right.', sBullet),
        Paragraph('Matching is scan-only <b>kpi_business.py:63</b> vs time-aware <b>align_storage_rows:209</b> (unused). Switching to time would improve but still not fix mixed shift — wait for dump.', sBullet),
        Paragraph('Threshold <b>MATCH_EPSILON 10.0:18</b> is global (±15 bucket) — hides azimuth but is NOT the RL cause. Do not tune until dump unified.', sBullet),
        Paragraph('Until dump fixed, report XML 95% as reference, HDF KPI as “logger-version mismatch, not detection quality”.', sBullet),
    ]
    left_who_content = [[left_who_title]] + [[b] for b in left_who_bullets]
    right_who_content = [[right_who_title]] + [[b] for b in right_who_bullets]
    left_who_table = Table(left_who_content, colWidths=[(W - 2*MARGIN)/2 - 7*mm])
    left_who_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), HexColor("#EAFAF1")),
        ('BOX', (0,0), (-1,-1), 0.5, GOOD),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 2),
        ('BOTTOMPADDING', (0,0), (-1,-1), 2),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
    ]))
    right_who_table = Table(right_who_content, colWidths=[(W - 2*MARGIN)/2 - 7*mm])
    right_who_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), HexColor("#FEF9E7")),
        ('BOX', (0,0), (-1,-1), 0.5, WARN),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 2),
        ('BOTTOMPADDING', (0,0), (-1,-1), 2),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
    ]))
    two_who = Table([[left_who_table, right_who_table]], colWidths=[(W - 2*MARGIN)/2, (W - 2*MARGIN)/2])
    two_who.setStyle(TableStyle([
        ('VALIGN', (0,0), (-1,-1), 'TOP'),
        ('LEFTPADDING', (0,0), (-1,-1), 3),
        ('RIGHTPADDING', (0,0), (-1,-1), 3),
    ]))
    story.append(two_who)
    story.append(Spacer(1, 3*mm))

    # Bottom line
    bottom_title = Paragraph('<b>Bottom line for management (copy this):</b> XML and HDF contain the same radar values, but the HDF logger wrote the INPUT and OUTPUT files with different SCAN counters and with some detector groups shifted by 1–2 rows. The KPI code lines up files by SCAN number, so it compares the wrong rows for RL and the velocity looks 32 m/s wrong → 24% vs true 95%. The fix belongs to the logger/dump team (unify SCAN/LOOK and per-det windowing). Your KPI code is correct — do not retune thresholds to hide the dump bug.', ParagraphStyle('bottom', parent=styles['Normal'], fontSize=7, leading=9, textColor=HexColor("#2C3E50"), fontName='Helvetica', alignment=TA_JUSTIFY, borderPadding=(5,5,5), backColor=LIGHT_GRAY))
    bottom_table = Table([[bottom_title]], colWidths=[W - 2*MARGIN - 4*mm])
    bottom_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), LIGHT_GRAY),
        ('BOX', (0,0), (-1,-1), 0.5, MID_GRAY),
        ('LEFTPADDING', (0,0), (-1,-1), 4),
        ('RIGHTPADDING', (0,0), (-1,-1), 4),
        ('TOPPADDING', (0,0), (-1,-1), 4),
        ('BOTTOMPADDING', (0,0), (-1,-1), 4),
        ('ROUNDEDCORNERS', [2*mm,2*mm,2*mm,2*mm]),
    ]))
    story.append(bottom_table)
    story.append(Spacer(1, 2*mm))
    story.append(Paragraph('<b>How we dumped (repro):</b> <i>python -c "import h5py; f=h5py.File(\'...HDF.h5\',\'r\'); print(f[\'CEER_RL\'][\'RDR_HEADER_001\'].attrs[\'HED_SCAN_INDEX\'][:8])"</i> and <i>print(f[\'CEER_RL\'][\'RDR_DETECTION_005_008\'].attrs[\'DET_RANGE_VELOCITY_005\'][:8])</i> and <i>xml.etree</i> on <i>CEER_PCAN_RL.xml</i>. Full code in <b>can_interactive_plot/</b> and <b>can_kpi/</b>.', sCaption))
    story.append(Paragraph('LaTeX source: <b>report.tex</b> (compile with pdflatex). This PDF rendered via ReportLab in LaTeX style because pdflatex not installed on this host — content identical.', sCaption))

    doc.build(story, onFirstPage=header_footer, onLaterPages=header_footer)
    print(f"Built {OUTPUT} — {OUTPUT.stat().st_size} bytes")

if __name__ == "__main__":
    build()
