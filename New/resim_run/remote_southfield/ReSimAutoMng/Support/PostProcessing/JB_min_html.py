"""
Created on Monday May 12 2025
@author: d1cse7 (mandeep.singh1@aptiv.com)
@copyright: APTIV
"""

from input_data import *

def get_html_data():
    keys=list(html_Metrix_values.keys())
    values=list(html_Metrix_values.values())
    application=list(html_application_versions.keys())
    versions=list(html_application_versions.values())
    sensor=list(html_sensor_versions.keys())
    sen_ver=list(html_sensor_versions.values())
    log_ver=list(html_log_versions.values())
    print(sensor)
    
    
    HTML_Content = '''
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
    google.charts.load("current", {packages:["corechart"]});
    google.charts.setOnLoadCallback(drawPie3d);
    function drawPie3d()
    {
        var data = google.visualization.arrayToDataTable([
          ['Scenerio', 'Impact'],'''
    for i in range(1,len(values)-1):
          HTML_Content+= f'''["{keys[i]}",{values[i][0]}],'''
    HTML_Content+='''
          ]);
        var options =
        {
          title: 'Resim-Execution Yeild',
          is3D: true,
          colors: ['#abebc6', '#f5b7b1', '#f9e79f', '#f2f3f4', '#aed6f1', '#c0392b', '#0099ff', '#0066cc', '#abb2b9']
        };
        var chart = new google.visualization.PieChart(document.getElementById('piechart_3d'));
        chart.draw(data, options);
      }

    HTML_Content+='''
  </script>
<style>

body {font-family: Arial;}

/* Style the tab */
.tab {
  overflow: hidden;
  border-bottom: 1px solid #ccc;
  background-color: #ffffff;
}

/* Style the buttons inside the tab */
.tab button {
  background-color: #ffeedd;
  float: left;
  border: 1px solid #ddd;
  outline: none;
  cursor: pointer;
  padding: 10px 10px;
  transition: 0.3s;
  font-size: 14px;
}

/* Change background color of buttons on hover */
.tab button:hover {
  background-color: #ddd;
}

/* Create an active/current tablink class */
.tab button.active {
  background-color: #ccc;
}

/* Style the tab content */
.tabcontent {
  display: none;
  padding: 6px 12px;
  border: 0px solid #ccc;
  background-color: #fff;
}

th, td {
  border:1px solid grey;
  background-color: #f8f9f9;
}
</style>

</head>
<body>

<p align="right" style="font-size:10px; color:red">Help : Mandeep.Singh1@aptiv.com</p>
<h2>ReSim Analysis</h2>
<!-- <p>Click on the buttons inside the tabbed menu:</p> -->

<div class="tab">
  <button class="tablinks" onclick="openCity(event, 'OverAll')" id="defaultOpen">OverAll-Yeild</button>
</div>
'''+f'''
<div id="OverAll" class="tabcontent">
  <table style="width:100%">
  <tr>
    <th style="width:70%; background-color:#aed6f1">Yeild : {values[0][0]} Logs</th>
    <th style="width:30%; background-color:#aed6f1" colspan=4>Impact</th>
  </tr>
  <tr>
    <td rowspan="{13+versions[-1]}"><div id="piechart_3d" style="width: 100%; height: 500px;" padding-right="0"></div></td>
    <th style="background-color:#d1f2eb">Factor</th>
    <th style="background-color:#d1f2eb">Yeild</th>
    <th style="background-color:#d1f2eb">Logs_Impacted </th>
    <th style="background-color:#d1f2eb">Session_Count</th>	
  </tr>
'''
    for i in range(1,len(values)-1):
        HTML_Content+=f'''<tr>
    <td>{keys[i]}</td>
    <td align="center">{round(100*values[i][0]/values[0][0],2)}</td>
    <td align="center">{values[i][0]}</td>
    <td align="center">{values[i][1]}</td>
  </tr>'''
    HTML_Content+=f'''
  <tr>
  </tr>

  <tr>
    <th style="width:15%; background-color:#aed6f1" colspan="2">Application</th>
    <th style="width:15%; background-color:#aed6f1" colspan="2">Version</th>
  </tr>'''
    for i in range(0,len(application)-1):
        HTML_Content+=f'''<tr>
    <td width="20%" colspan="2">{application[i]}</td>
    <td align="center" width="20%" colspan="2">{versions[i]}</td>
  </tr>
  '''
    HTML_Content+='''
  <tr>
    <th style="width:10%; background-color:#aed6f1" colspan="1">Sensor</th>
    <th style="width:10%; background-color:#aed6f1" colspan="1">Software</th>
    <th style="width:10%; background-color:#aed6f1" colspan="2">Log</th>
  </tr>'''
    for i in range (0,len(sensor)):
        print(sensor[i])
        HTML_Content+=f'''<tr>
    <td width="10%" colspan="1">{sensor[i]}</td>
    <td align="center" width="10%" colspan="1">{sen_ver[i]}</td>'''
        if sensor[i] != "DC_SRR" and sensor[i] != "DC_MRR" : HTML_Content+=f'''
    <td align="center" width="10%" colspan="2">{log_ver[i-2]}</td>'''
        else:HTML_Content+=f'''
    <td align="center" width="10%" colspan="2"></td>'''
        HTML_Content+='''
  </tr>
    '''
    
    HTML_Content+='''
</table>
<div style="height:3000px"></div>
</div>

<script>'''+'''
function openCity(evt, cityName) {
  var i, tabcontent, tablinks;
  tabcontent = document.getElementsByClassName("tabcontent");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }
  tabcontent[0].style.display = "block";
  tablinks = document.getElementsByClassName("tablinks");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" active", "");
  }
  document.getElementById(cityName).style.display = "block";
  evt.currentTarget.className += " active";
}
document.getElementById("defaultOpen").click();
</script>
   
</body>
</html> 
'''
    return HTML_Content
