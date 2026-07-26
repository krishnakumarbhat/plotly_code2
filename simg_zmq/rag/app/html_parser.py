from __future__ import annotations

import re
from pathlib import Path
from typing import Any

from bs4 import BeautifulSoup


class HtmlParser:
    def __init__(self, english_only: bool = True) -> None:
        self.english_only = english_only

    def parse_file(self, file_path: Path) -> str:
        parsed = self.parse_file_sections(file_path)
        return parsed['text']

    def parse_file_sections(self, file_path: Path) -> dict[str, Any]:
        raw_text = file_path.read_text(encoding='utf-8', errors='ignore')
        if file_path.suffix.lower() in {'.html', '.htm', '.xml'}:
            sections, metadata = self._parse_html_sections(raw_text, file_path)
        else:
            normalized = self._normalize_text(raw_text)
            metadata = self._build_document_metadata(file_path, file_path.stem, normalized)
            sections = []
            if normalized:
                sections.append(
                    {
                        'title': metadata['document_title'],
                        'kind': 'text',
                        'text': normalized,
                        'metadata': metadata,
                    }
                )

        full_text = self._normalize_text(' '.join(section['text'] for section in sections))
        return {
            'text': full_text,
            'sections': sections,
            'metadata': metadata,
        }

    def parse_html(self, html: str) -> str:
        file_stub = Path('document.html')
        sections, _ = self._parse_html_sections(html, file_stub)
        return self._normalize_text(' '.join(section['text'] for section in sections))

    def _parse_html_sections(self, html: str, file_path: Path) -> tuple[list[dict[str, Any]], dict[str, Any]]:
        soup = BeautifulSoup(html, 'html.parser')
        for element in soup(['script', 'style', 'noscript', 'svg', 'footer', 'nav', 'header']):
            element.decompose()

        document_title = self._normalize_text(soup.title.get_text(' ', strip=True) if soup.title else file_path.stem)
        body = soup.body or soup
        sections: list[dict[str, Any]] = []
        current_title = document_title or file_path.stem
        current_parts: list[str] = []

        def flush_section() -> None:
            if not current_parts:
                return
            text = self._normalize_text(' '.join(current_parts))
            if not text:
                current_parts.clear()
                return
            section_metadata = self._build_document_metadata(file_path, current_title, text)
            sections.append(
                {
                    'title': current_title,
                    'kind': 'section',
                    'text': text,
                    'metadata': section_metadata,
                }
            )
            current_parts.clear()

        for element in body.find_all(['h1', 'h2', 'h3', 'h4', 'table', 'p', 'li', 'pre', 'code']):
            tag_name = element.name.lower()
            if tag_name in {'p', 'li'} and element.find_parent(['table']):
                continue

            if tag_name in {'h1', 'h2', 'h3', 'h4'}:
                flush_section()
                heading = self._normalize_text(element.get_text(' ', strip=True))
                if heading:
                    current_title = heading
                continue

            if tag_name == 'table':
                flush_section()
                table_text = self._table_to_text(element)
                if table_text:
                    section_metadata = self._build_document_metadata(file_path, current_title, table_text)
                    sections.append(
                        {
                            'title': current_title,
                            'kind': 'table',
                            'text': table_text,
                            'metadata': section_metadata,
                        }
                    )
                continue

            text = self._normalize_text(element.get_text(' ', strip=True))
            if not text:
                continue
            current_parts.append(text)
            if len(' '.join(current_parts)) >= 1600:
                flush_section()

        flush_section()
        if not sections:
            fallback_text = self._normalize_text(body.get_text(' ', strip=True))
            metadata = self._build_document_metadata(file_path, document_title or file_path.stem, fallback_text)
            if fallback_text:
                sections.append(
                    {
                        'title': metadata['document_title'],
                        'kind': 'section',
                        'text': fallback_text,
                        'metadata': metadata,
                    }
                )
        metadata = self._build_document_metadata(file_path, document_title or file_path.stem, ' '.join(section['text'] for section in sections[:3]))
        return sections, metadata

    def _table_to_text(self, table) -> str:
        rows: list[str] = []
        for row in table.find_all('tr'):
            cells = [self._normalize_text(cell.get_text(' ', strip=True)) for cell in row.find_all(['th', 'td'])]
            cells = [cell for cell in cells if cell]
            if cells:
                rows.append(' | '.join(cells))
        return self._normalize_text(' ; '.join(rows))

    def _build_document_metadata(self, file_path: Path, document_title: str, sample_text: str) -> dict[str, Any]:
        report_kind = self._report_kind(file_path)
        scenario_tags = self._extract_scenario_tags(f'{file_path.name} {document_title} {sample_text}')
        return {
            'source_name': file_path.name,
            'document_title': document_title or file_path.stem,
            'report_kind': report_kind,
            'scenario_tags': scenario_tags,
        }

    @staticmethod
    def _report_kind(file_path: Path) -> str:
        lower_name = file_path.name.lower()
        if 'l_sil_validation_report' in lower_name:
            return 'sil_validation_report'
        if 'validation' in lower_name:
            return 'validation_report'
        if 'scenario' in lower_name:
            return 'scenario_report'
        if 'log' in lower_name:
            return 'runtime_log'
        return 'html_report'

    def _extract_scenario_tags(self, text: str) -> list[str]:
        lowered = text.lower()
        tags: list[str] = []
        for match in re.findall(r'(truck[^\n\r,;:.]{0,64})', lowered):
            normalized = self._normalize_text(match)
            if normalized and normalized not in tags:
                tags.append(normalized)
        for match in re.findall(r'(scenario\s+[a-z0-9_\-]{1,48})', lowered):
            normalized = self._normalize_text(match)
            if normalized and normalized not in tags:
                tags.append(normalized)
        return tags[:8]

    def _normalize_text(self, text: str) -> str:
        text = re.sub(r'\s+', ' ', text).strip()
        if self.english_only:
            text = self._filter_english_text(text)
        return text

    @staticmethod
    def _filter_english_text(text: str) -> str:
        filtered = re.sub(r"[^A-Za-z0-9\s\.,;:!?@#%&\(\)\[\]\{\}\-_\\/\\'\"\+=\*<>\|`~$]", ' ', text)
        filtered = re.sub(r'\s+', ' ', filtered).strip()
        return filtered
