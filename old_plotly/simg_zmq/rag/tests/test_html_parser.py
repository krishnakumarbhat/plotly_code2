from app.html_parser import HtmlParser


class TestHtmlParser:
    def test_parse_html_removes_noise(self) -> None:
        parser = HtmlParser()
        html = """
        <html>
          <head><style>.x{color:red}</style></head>
          <body>
            <script>console.log('x')</script>
            <h1>Title</h1>
            <p>Some <b>text</b></p>
          </body>
        </html>
        """
        output = parser.parse_html(html)
        assert "Title" in output
        assert "Some text" in output
        assert "console.log" not in output
