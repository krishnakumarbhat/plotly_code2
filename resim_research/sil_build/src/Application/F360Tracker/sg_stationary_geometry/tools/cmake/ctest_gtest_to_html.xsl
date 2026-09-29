<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <html>
      <head>
        <title>GoogleTest Results</title>
        <style>
          body { font-family: Arial, sans-serif; }
          .pass { color: green; }
          .fail { color: red; font-weight: bold; }
          table { border-collapse: collapse; width: 80%; }
          th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
          th { background-color: #f2f2f2; }
        </style>
      </head>
      <body>
        <h1>GoogleTest Results</h1>
        <p>
          <strong>Total Tests: </strong> <xsl:value-of select="//testsuite/@tests"/>
          <br/>
          <strong>Failures: </strong> <xsl:value-of select="//testsuite/@failures"/>
          <br/>
          <strong>Disabled: </strong> <xsl:value-of select="//testsuite/@disabled"/>
          <br/>
          <strong>Skipped: </strong> <xsl:value-of select="//testsuite/@skipped"/>
          <br/>
          <strong>Time: </strong> <xsl:value-of select="//testsuite/@time"/> seconds
        </p>
        
        <div class="filter-container">
          <label for="columnSelect">Filter by:</label>
          <select id="columnSelect">
            <option value="1">Status</option>
          </select>
            
          <label for="valueSelect">Value:</label>
          <select id="valueSelect">
            <!-- Options will be populated dynamically -->
          </select>
            
          <button onclick="resetFilter()">Reset Filter</button>
        </div>       
        <h2>Test Results</h2>
        <table id="details_table">
          <tr>
            <th>Test Case</th>
            <th>Status</th>
            <th>Time (s)</th>
            <th>Details</th>
          </tr>
          <xsl:for-each select="//testsuite">
            <xsl:variable name="testsuite" select="@name"/>
            <xsl:for-each select="testcase">
              <tr>
                <td><xsl:value-of select="@name"/></td>
                <td>
                  <xsl:choose>
                    <xsl:when test="@status = 'fail'">
                      <span class="fail">FAIL</span>
                    </xsl:when>
                    <xsl:when test="@status = 'run'">
                      <span class="pass">PASS</span>
                    </xsl:when>
                    <xsl:when test="@status = 'disabled'">
                      <span>DISABLED</span>
                    </xsl:when>
                    <xsl:otherwise>
                      <span>UNKNOWN</span>
                    </xsl:otherwise>
                  </xsl:choose>
                </td>
                <td><xsl:value-of select="@time"/></td>
                <td>
                  <xsl:if test="failure">
                    <pre><xsl:value-of select="system-out"/></pre>
                  </xsl:if>
                </td>
              </tr>
            </xsl:for-each>
          </xsl:for-each>
        </table>
        <script>
          <xsl:text disable-output-escaping="yes">
            <![CDATA[
              document.addEventListener('DOMContentLoaded', function() {
                populateValueDropdown();
                  
                document.getElementById('columnSelect').addEventListener('change', function() {
                  populateValueDropdown();
                });
                
                document.getElementById('valueSelect').addEventListener('change', function() {
                  filterTable();
                });
              });
              
              function populateValueDropdown() {
                const columnIndex = parseInt(document.getElementById('columnSelect').value);
                const valueSelect = document.getElementById('valueSelect');
                const table = document.getElementById('details_table');
                const rows = table.getElementsByTagName('tr');
                
                valueSelect.innerHTML = '<option value="">All</option>';
                
                const uniqueValues = new Set();
                
                // Start from index 1 to skip the header row
                for (let i = 1; i < rows.length; i++) {
                  const cell = rows[i].getElementsByTagName('td')[columnIndex];
                  if (cell) {
                    uniqueValues.add(cell.textContent.trim());
                  }
                }
                
                uniqueValues.forEach(value => {
                  const option = document.createElement('option');
                  option.value = value;
                  option.textContent = value;
                  valueSelect.appendChild(option);
                });
              }
              
              function filterTable() {
                const columnIndex = parseInt(document.getElementById('columnSelect').value);
                const filterValue = document.getElementById('valueSelect').value;
                const table = document.getElementById('details_table');
                const rows = table.getElementsByTagName('tr');
                
                // Start from index 1 to skip the header
                for (let i = 1; i < rows.length; i++) {
                  const cell = rows[i].getElementsByTagName('td')[columnIndex];
                  
                  if (cell) {
                    const textValue = cell.textContent || cell.innerText;
                    
                    if (filterValue === '' || textValue.trim() === filterValue) {
                      rows[i].style.display = '';
                    } else {
                      rows[i].style.display = 'none';
                    }
                  }
                }
              }
              
              function resetFilter() {
                document.getElementById('valueSelect').value = '';
                
                const table = document.getElementById('details_table');
                const rows = table.getElementsByTagName('tr');
                
                for (let i = 1; i < rows.length; i++) {
                  rows[i].style.display = '';
                }
              }         
            ]]>            
          </xsl:text>
        </script>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
