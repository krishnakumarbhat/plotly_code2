"""
Created on Monday May 12 2025
@author: d1cse7 (mandeep.singh1@aptiv.com)
@copyright: APTIV
"""

from input_data import *

def slowness_summary(dataofinterest):
    valid=[]
    
    for value in dataofinterest:
        if value != -1: valid.append(value)
    try:avgJ=round(sum(valid)/len(valid),2)
    except: avgJ=0
    
    if len(valid) > 0: maxrate=max(valid); minrate=min(valid)
    else:maxrate=minrate=0
    
    return f''' <td style="width:15%; font-size: 14px;">
                  <p>Max-Execution-Time: {maxrate}</p>
                  <p>Min-Execution-Time: {minrate}</p>
                  <p style="color:green">Avg-Execution-Time: {avgJ}</p>
                  <br>
                  <p>Total Session        : {len(dataofinterest)}</p>
                  <p style="color:green">Session Completed    : {len(valid)}</p>
                  <p style="color:red">Session Not Completed : {len(dataofinterest) - len(valid)}</p>
              </td>'''

def linechart_props(slowness,color, slow_id):
    return ''']);

        const options = {''' + f'''
          title: 'Session vs. {slowness}','''+'''
          hAxis: {title: 'Sessions'},
          vAxis: {title: 'Slowness Factor'},
          width: 1600,
          height: 300,
          legend: 'true',
          colors:'''+f'''{color}'''+'''
                        };'''+f'''
        const chart = new google.visualization.LineChart(document.getElementById('{slow_id}'));'''+'''
        chart.draw(data, options);
    }'''


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

    google.charts.setOnLoadCallback(drawslowOverall);
    function drawslowOverall()
    {
        const data = google.visualization.arrayToDataTable([
          ['Session', 'Job', 'ReSim', 'HTML', 'BORD_I', 'BORD_O', 'MUDP_I', 'MUDP_O'],'''
    cnt=1
    if len(html_Slowverall) > 0:
        for i in html_Slowverall:
            HTML_Content+=f'''["{cnt}"'''
            for val in i:
                HTML_Content+=f''',{val}'''
            HTML_Content+='''],'''
            cnt+=1
    else : HTML_Content+='''["0",0,0,0,0,0,0],'''
    HTML_Content+='''
        ]);

        const options = {
          title: 'Session vs. Slowness',
          hAxis: {title: 'Sessions'},
          vAxis: {title: 'Slowness Factor'},
          width: 1840,
          height: 300,
          legend: 'true',
          colors:["#17202a","#58d68d","#c0392b","#d4ac0d","#d4000d","#3498db","#2400ff"]
                        };
        const chart = new google.visualization.LineChart(document.getElementById('slowness_overall'));
        chart.draw(data, options);
    }

    google.charts.setOnLoadCallback(drawslowJob);
    function drawslowJob()
    {
        const data = google.visualization.arrayToDataTable([
          ['Session',"slowness"],'''
    cnt=1
    if len(html_SlowJob) > 0:
        for i in html_SlowJob:
            HTML_Content+=f'''["{cnt}",{i}],'''
            cnt+=1
    else : HTML_Content+='''["0",0],'''
    HTML_Content+=linechart_props("Job_Slowness",['#17202a'],"slowness_job" )


#********************* ReSim line graph
    HTML_Content+='''

    google.charts.setOnLoadCallback(drawslowResim);
    function drawslowResim()
    {
        const data = google.visualization.arrayToDataTable([
          ['Session',"slowness"],'''
    cnt=1
    if len(html_SlowReSim) > 0:
        for i in html_SlowReSim:
            HTML_Content+=f'''["{cnt}",{i}],'''
            cnt+=1
    else : HTML_Content+='''["0",0],'''
    HTML_Content+=linechart_props("ReSim_Slowness",['#58d68d'],"slowness_resim")


#********************* HTML line graph
    HTML_Content+='''

    google.charts.setOnLoadCallback(drawslowHtml);
    function drawslowHtml()
    {
        const data = google.visualization.arrayToDataTable([
          ['Session',"slowness"],'''
    cnt=1
    if len(html_SlowHTML) > 0:
        for i in html_SlowHTML:
            HTML_Content+=f'''["{cnt}",{i}],'''
            cnt+=1
    else : HTML_Content+='''["0",0],'''
    HTML_Content+=linechart_props("HTML_Slowness",['#c0392b'],"slowness_html")


#********************* BORDNET -Input line graph
    HTML_Content+='''

    google.charts.setOnLoadCallback(drawslowIBORD);
    function drawslowIBORD()
    {
        const data = google.visualization.arrayToDataTable([
          ['Session',"slowness"],'''
    cnt=1
    if len(html_SlowIBORD) > 0:
        for i in html_SlowIBORD:
            HTML_Content+=f'''["{cnt}",{i}],'''
            cnt+=1
    else : HTML_Content+='''["0",0],'''
    HTML_Content+=linechart_props("Bordnet_Input_Slowness",['#d4ac0d'],"slowness_bord_input")


#********************* BORDNET -Output line graph
    HTML_Content+='''

    google.charts.setOnLoadCallback(drawslowOBORD);
    function drawslowOBORD()
    {
        const data = google.visualization.arrayToDataTable([
          ['Session',"slowness"],'''
    cnt=1
    if len(html_SlowIBORD) > 0:
        for i in html_SlowIBORD:
            HTML_Content+=f'''["{cnt}",{i}],'''
            cnt+=1
    else : HTML_Content+='''["0",0],'''
    HTML_Content+=linechart_props("Bordnet_Output_Slowness",['#d4000d'],"slowness_bord_output")



#********************* MUDP -Input line graph
    HTML_Content+='''

    google.charts.setOnLoadCallback(drawslowIMUDP);
    function drawslowIMUDP()
    {
        const data = google.visualization.arrayToDataTable([
          ['Session',"slowness"],'''
    cnt=1
    if len(html_SlowIMUDP) > 0:
        for i in html_SlowIMUDP:
            HTML_Content+=f'''["{cnt}",{i}],'''
            cnt+=1
    else : HTML_Content+='''["0",0],'''
    HTML_Content+=linechart_props("MUDP_Input_Slowness",["#3498db"],"slowness_mudp_input")


#********************* MUDP -Input line graph
    HTML_Content+='''

    google.charts.setOnLoadCallback(drawslowOMUDP);
    function drawslowOMUDP()
    {
        const data = google.visualization.arrayToDataTable([
          ['Session',"slowness"],'''
    cnt=1
    if len(html_SlowOMUDP) > 0:
        for i in html_SlowOMUDP:
            HTML_Content+=f'''["{cnt}",{i}],'''
            cnt+=1
    else : HTML_Content+='''["0",0],'''
    HTML_Content+=linechart_props("MUDP_Output_Slowness",["#2400ff"],"slowness_mudp_output")



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
  <button class="tablinks" onclick="openCity(event, 'Mining')">Mining-Details</button>
  <button class="tablinks" onclick="openCity(event, 'Crash')">Crash-Details</button>
  <button class="tablinks" onclick="openCity(event, 'BadLog')">BadLog-Details</button>
  <button class="tablinks" onclick="openCity(event, 'Slowness')">Slowness-Analysis</button>
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

<div id="Mining" class="tabcontent" style="overflow-x:true">
    <table style="width:100%">
      <tr>
        <th rowspan="2" style="background-color:#d1f2ff">JobNo.</th>
        <th rowspan="2" style="background-color:#d1f2ff">Logs-Version</th>
        <th rowspan="2" style="background-color:#d1f2ff">Session</th>
	<th rowspan="2" style="background-color:#d1f2ff">SIL Mode</th>
	<th rowspan="2" style="background-color:#d1f2ff">Files Mode</th>
        <th rowspan="2" style="background-color:#d1f2ff">Duration (sec)</th>
        <th rowspan="2" style="background-color:#d1f2ff">Yeild</th>
        <th colspan="7" style="background-color:#d1f2eb">Status</th>
        <th colspan="5" style="background-color:#d7bde2">Logs-Details</th>
        <th colspan="7" style="background-color:#e6b0aa">Execution-Time</th>
        <th colspan="7" style="background-color:#abb2b9">Slowness</th>
        <th rowspan="2" style="background-color:#d1f2ff">Bad Log</th>
        <th rowspan="2" style="background-color:#d1f2ff">Session-Details</th>
        <tr>
          <th style="background-color:#d1f2eb"><pre> Job </pre></th>
          <th style="background-color:#d1f2eb"><pre> ReSim </pre></th>
          <th style="background-color:#d1f2eb"><pre> HTML </pre></th>
          <th style="background-color:#d1f2eb"><pre> BORD-I </pre></th>
          <th style="background-color:#d1f2eb"><pre> BORD-O </pre></th>
          <th style="background-color:#d1f2eb"><pre> MUDP-I </pre></th>
          <th style="background-color:#d1f2eb"><pre> MUDP-O </pre></th>

          <th style="background-color:#d7bde2"><pre> Total </pre></th>
          <th style="background-color:#d7bde2"><pre> ReSimulated </pre></th>
          <th style="background-color:#d7bde2"><pre> Not_ReSimulated </pre></th>
          <th style="background-color:#d7bde2"><pre> Bad </pre></th>
          <th style="background-color:#d7bde2"><pre> Crash </pre></th>

          <th style="background-color:#e6b0aa"><pre> Job </pre></th>
          <th style="background-color:#e6b0aa"><pre> ReSim </pre></th>
          <th style="background-color:#e6b0aa"><pre> HTML </pre></th>
          <th style="background-color:#e6b0aa"><pre> BORD-I </pre></th>
          <th style="background-color:#e6b0aa"><pre> BORD-O </pre></th>
          <th style="background-color:#e6b0aa"><pre> MUDP-I </pre></th>
          <th style="background-color:#e6b0aa"><pre> MUDP-O </pre></th>

          <th style="background-color:#abb2b9"><pre> Job </pre></th>
          <th style="background-color:#abb2b9"><pre> ReSim </pre></th>
          <th style="background-color:#abb2b9"><pre> HTML </pre></th>
          <th style="background-color:#abb2b9"><pre> BORD-I </pre></th>
          <th style="background-color:#abb2b9"><pre> BORD-O </pre></th>
          <th style="background-color:#abb2b9"><pre> MUDP-I </pre></th>
          <th style="background-color:#abb2b9"><pre> MUDP-O </pre></th>
        </tr>
    </tr>
'''
    for info in html_mining_data:
        HTML_Content += "    <tr>\n"
        for i in info:HTML_Content +=f"       <td><pre> {i} </pre></td>\n"
        HTML_Content+="    </tr>\n"
    HTML_Content+=f'''
    </table>
    <div style="height:3000px"></div>
</div>

<div id="Crash" class="tabcontent">
  <table style="width:100%">
    <tr>
      <th style="background-color:#d1f2ff">Crashed Session Details : {len(html_crashLog)} sessions</th>
    </tr>'''
    for i in html_crashLog:
        HTML_Content+=f'''    <tr><td><pre> {i}</pre></td></tr>\n
'''
    HTML_Content+=f'''    </table>
    <div style="height:3000px"></div>
</div>

<div id="BadLog" class="tabcontent">
  <table style="width:100%">
    <tr>
      <th width=80% rowspan="2" style="background-color:#d1f2ff">Bad Log Details :  {len(html_badLog)} logs</th>
      <th width=20% colspan="2"style="background-color:#d1f2ff">DataQuality Status</th>
    </tr>
    <tr>
      <th width=10% style="background-color:#d1f2ff">ReSim</th>
      <th width=10% style="background-color:#d1f2ff">Log Actual Quality</th>
    </tr>
    '''
    for i in html_badLog:
        HTML_Content+=f'''    <tr><td><pre> {i}</pre></td><td><pre> {i[1]} </pre></td><td><pre> {i[2]} </pre></td></tr>\n
'''
    HTML_Content+=f'''    </table>
  </table>
  <div style="height:3000px"></div>
</div>

<div id="Slowness" class="tabcontent">
    <div style="overflow-x:auto">
        <table style="width:100%">
            <tr><th style="width:100%; background-color:#d1f2ff">Execution Slowness</th></tr>
            <tr>
              <td ><div id="slowness_overall" style="width: 100%; height: auto;"></div></td>
            </tr>
        </table>
    </div>
    <br>
    <div style="overflow-x:auto">
        <table style="width:100%">
            <tr><th colspan="2" style="width:100%; background-color:#d1f2ff">Job Slowness</th></tr>
            <tr>
              <td><div id="slowness_job" style="width: 85%; height: auto;" padding-right="0"></div></td>
              {slowness_summary(html_SlowJob)}
            </tr>
        </table>
    </div>
    <br>
    <div style="overflow-x:auto">
        <table style="width:100%">
            <tr><th colspan="2" style="width:100%; background-color:#d1f2ff">Resim Slowness</th></tr>
            <tr>
              <td ><div id="slowness_resim" style="width: 85%; height: auto;" padding-right="0"></div></td>
              {slowness_summary(html_SlowReSim)}
            </tr>
        </table>
    </div>
    <br>
    <div style="overflow-x:auto">
        <table style="width:100%">
            <tr><th colspan="2" style="width:100%; background-color:#d1f2ff">HTML Slowness</th></tr>
            <tr>
              <td ><div id="slowness_html" style="width: 100%; height: auto;" padding-right="0"></div></td>
              {slowness_summary(html_SlowHTML)}
            </tr>
        </table>
    </div>
    <br>
    <div style="overflow-x:auto">
        <table style="width:100%">
            <tr><th colspan="2" style="width:100%; background-color:#d1f2ff">BORDNET INPUT Slowness</th></tr>
            <tr>
              <td ><div id="slowness_bord_input" style="width: 85%; height: auto;" padding-right="0"></div></td>
              {slowness_summary(html_SlowIBORD)}
            </tr>
        </table>
    </div>
    <br>
    <div style="overflow-x:auto">
        <table style="width:100%">
            <tr><th colspan="2" style="width:100%; background-color:#d1f2ff">BORDNET Output Slowness</th></tr>
            <tr>
              <td ><div id="slowness_bord_output" style="width: 85%; height: auto;" padding-right="0"></div></td>
              {slowness_summary(html_SlowOBORD)}
            </tr>
        </table>
    </div>
    <br>

    <div style="overflow-x:auto">
        <table style="width:100%">
            <tr><th colspan="2" style="width:100%; background-color:#d1f2ff">MUDP Input Slowness</th></tr>
            <tr>
              <td ><div id="slowness_mudp_input" style="width: 85%; height: auto;" padding-right="0"></div></td>
              {slowness_summary(html_SlowIMUDP)}
            </tr>
        </table>
    </div>
    <br>
    <div style="overflow-x:auto">
        <table style="width:100%">
            <tr><th colspan="2" style="width:100%; background-color:#d1f2ff">MUDP Output Slowness</th></tr>
            <tr>
              <td ><div id="slowness_mudp_output" style="width: 85%; height: auto;" padding-right="0"></div></td>
              {slowness_summary(html_SlowOMUDP)}
            </tr>
        </table>
    </div>

</div>

<script>'''+'''
function openCity(evt, cityName) {
  var i, tabcontent, tablinks;
  tabcontent = document.getElementsByClassName("tabcontent");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }
  tabcontent[4].style.display = "block";
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
