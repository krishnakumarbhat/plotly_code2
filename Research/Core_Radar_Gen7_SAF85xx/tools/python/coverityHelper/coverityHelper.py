"""

Parse a given Coverity reports and save the data for use in metric reporting tools.

This python script requires the following Synopsys Coverity reports for a specific Coverity stream.
These reports are all comma seperated csv or txt files with the first row being the column headers.
This input is categorized and sent to Datadog to populate dashboards.

Issue List Report:
    - Description: Coverity Issues report for all issues regardless of status/impact/Misra Category/Component/Count
    - Required headers: "Impact", "Standard: MISRA Category", "Status", "Component", "Count"

Component List Report:
    - Description: Coverity Components report for all issues in each component including zeros.
    - Required headers: "Component"

CCM (Cyclomatic Complexity Metric) Report:
    - Description: Coverity Function report for CCM violations > 1
    - Required headers: "Component", "CCM"

"""

from os import path, strerror
import argparse
import errno
from tools.python.dataDogMetrics import dataDogStats


class covHelper:
    """
    Parse a given Coverity report and save the data for use in metric reporting tools.
    """

    def __init__(self, uploadToDataDog=False, branch=None):
        """
        Create the object and define if metrics should be uploaded to DataDog.

        Args:
            uploadToDataDog: Flag to tell the class to upload the metrics to DataDog. Default=False
            branch: A branch name to tag the metrics with. Default=None
        """
        self.misra_categories = {
            "Mandatory": {"New": {}},
            "Required": {"New": {}},
            "Advisory": {"New": {}},
            "None": {"New": {}},
        }
        self.grouped_issues = {
            "High": {"None": {"New": {}}},
            "Medium": {"None": {"New": {}}},
            "Low": self.misra_categories,
        }
        # Adding "Various" since the component report will not have it and
        # the issues report will give the category of "Various" for some issues
        self.component_list = {"Various": 0}
        self.grouped_ccm = {
            "CCM_1_to_15": {},
            "CCM_16_to_50": {},
            "CCM_51_plus": {},
        }
        self.uploadToDataDog = uploadToDataDog
        self.dataDogMetrics = dataDogStats(branch=branch)

    def readCovReports(
        self, compListReport, allDefectsReport, ccmReport, reportName="", swVersion=""
    ):
        """
        Read the report files and put them into data frames to process and send to datadog.

        Args:
            compListReport: Report from Coverity containing all the possible components.
            allDefectsReport: Report from Coverity containing all issues for a stream.
            ccmReport: Report from Coverity containing the CCM (Cyclomatic Complexity Metric)
            reportName: Name to use for variant in datadog tag.
        """
        import pandas as pd

        if not path.exists(allDefectsReport):
            print("Coverity file not found: '%s'" % (allDefectsReport))
            raise FileNotFoundError(errno.ENOENT, strerror(errno.ENOENT), allDefectsReport)
        if not path.exists(compListReport):
            print("Coverity file not found: '%s'" % (compListReport))
            raise FileNotFoundError(errno.ENOENT, strerror(errno.ENOENT), compListReport)
        if not path.exists(ccmReport):
            print("Coverity file not found: '%s'" % (ccmReport))
            raise FileNotFoundError(errno.ENOENT, strerror(errno.ENOENT), ccmReport)

        if not reportName:
            reportName = ""
        if not swVersion:
            swVersion = ""

        # Read the list of all components and add to the dictionary.
        # Assign 0 to all of them initially and increment later
        if path.exists(compListReport):
            data = pd.read_csv(compListReport)
            df = pd.DataFrame(data, columns=["Component"])
            data = df.groupby(["Component"]).size()
            for comp, _count in data.items():
                self.component_list[comp] = 0

        # Set a zero for each component in every impact/misra category/status
        # this help display zeros in datadog, otherwise there is no data to be displayed
        for impact in self.grouped_issues:
            for misraCategory in self.grouped_issues[impact]:
                for status in self.grouped_issues[impact][misraCategory]:
                    self.grouped_issues[impact][misraCategory][status] = dict(self.component_list)

        cov_data = pd.read_csv(allDefectsReport)

        # list of expected headers
        headers = ["Impact", "Standard: MISRA Category", "Status", "Component", "Count"]

        for header in headers:
            if header not in cov_data:
                raise LookupError(
                    "Coverity All Issues Report file does not have all expected columns."
                )

        # Create a small data frame with only the needed columns
        cov_data = pd.DataFrame(cov_data, columns=headers)

        # group by the provided headers.
        cov_data = cov_data.groupby(headers).size()

        # Iterate through the panda dataframe and add up the issue count for each component
        for row, count in cov_data.items():
            result = dict(zip(headers, row))

            impact = result["Impact"]
            misraCategory = result["Standard: MISRA Category"]
            status = result["Status"]
            component = result["Component"]
            issueCnt = result["Count"] * count

            try:
                self.grouped_issues[impact][misraCategory][status][component] += issueCnt
            except KeyError:
                self.grouped_issues[impact][misraCategory][status] = dict(self.component_list)
                self.grouped_issues[impact][misraCategory][status][component] += issueCnt

        if self.uploadToDataDog:
            # Iterate through the gouped_issues and send to datadog
            for impact, misraCategoryDict in self.grouped_issues.items():
                for misraCategory, coverityIssueStatusDict in misraCategoryDict.items():
                    for status, componentDict in coverityIssueStatusDict.items():
                        if not componentDict:
                            # Add blank component for datadog so it can report 0 for this status
                            componentDict = {"None": 0}
                        for component, issueCnt in componentDict.items():
                            tags = [
                                "gen7_v2_env:test",
                                "Impact:" + impact,
                                "MISRA_Category:" + misraCategory,
                                "Status:" + status,
                                "Component:" + component,
                                "Variant:" + reportName,
                            ]

                            if swVersion:
                                tags.append("SW_Version:" + swVersion)

                            self.dataDogMetrics.sendGaugeMetric(
                                "advradar.gen7_v2.Coverity.Detailed",
                                issueCnt,
                                tags,
                            )

        ccm_data = pd.read_csv(ccmReport)
        # list of expected headers
        headers = ["Component", "CCM"]

        for header in headers:
            if header not in ccm_data:
                raise LookupError("Coverity CCM Report file does not have all expected columns.")

        # Create a small data frame with only the needed columns for CCM data
        ccm_data = pd.DataFrame(ccm_data, columns=headers)
        # group by the provided headers.
        ccm_data = ccm_data.groupby(headers).size()

        # Iterate through the panda dataframe and add up the CCM issues in reach category
        for row, count in ccm_data.items():
            result = dict(zip(headers, row))

            component = result["Component"]
            ccm = result["CCM"]

            if ccm <= 15:
                try:
                    self.grouped_ccm["CCM_1_to_15"][component] += count
                except KeyError:
                    self.grouped_ccm["CCM_1_to_15"][component] = count
            elif (ccm > 15) and (ccm <= 50):
                try:
                    self.grouped_ccm["CCM_16_to_50"][component] += count
                except KeyError:
                    self.grouped_ccm["CCM_16_to_50"][component] = count
            elif ccm > 50:
                try:
                    self.grouped_ccm["CCM_51_plus"][component] += count
                except KeyError:
                    self.grouped_ccm["CCM_51_plus"][component] = count
            else:
                continue

        if self.uploadToDataDog:
            # Iterate through the grouped_ccm and send to datadog
            for ccm_group, componentDict in self.grouped_ccm.items():
                if not componentDict:
                    # Add blank component for datadog so it can report zero for this ccm_group
                    componentDict = {"None": 0}
                for component, issueCnt in componentDict.items():
                    tags = [
                        "gen7_v2_env:test",
                        "CCM_Group:" + ccm_group,
                        "Component:" + component,
                        "Variant:" + reportName,
                    ]
                    if swVersion:
                        tags.append("SW_Version:" + swVersion)

                    self.dataDogMetrics.sendGaugeMetric(
                        "advradar.gen7_v2.Coverity.CyclomaticComplexity",
                        issueCnt,
                        tags,
                    )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Provide the coverity report with components and number of issues"
    )
    parser.add_argument(
        "--allDefectsReport",
        dest="allDefectsReport",
        action="store",
        default="",
        required=True,
        help="Provide the full path to the Coverity All Issues Component report",
    )
    parser.add_argument(
        "--compListReport",
        dest="compListReport",
        action="store",
        default="",
        required=True,
        help="Provide the full path to the Coverity Components report",
    )
    parser.add_argument(
        "--ccmReport",
        dest="ccmReport",
        action="store",
        default="",
        required=True,
        help="Provide the full path to the Coverity CCM report",
    )
    parser.add_argument(
        "--reportName",
        dest="reportName",
        action="store",
        default="",
        help="Provide a file report name, ex: 'srr6p_mss'. This will be a suffix on the output"
        "files, as well as a tag for the dataDog metrics",
    )
    parser.add_argument(
        "--dataDog",
        dest="uploadToDataDog",
        action="store_true",
        default=False,
        help="Include to upload the results to DataDog. Will only work on Jenkins",
    )
    parser.add_argument(
        "--branch",
        dest="branch",
        type=str,
        action="store",
        default=None,
        help="A branch to tag the DataDog metrics with",
    )
    parser.add_argument(
        "--swVersion",
        dest="swVersion",
        type=str,
        action="store",
        default=None,
        help="A sw version release tag to tag the DataDog metrics with",
    )
    args = parser.parse_args()

    cov_helper = covHelper(uploadToDataDog=args.uploadToDataDog, branch=args.branch)
    cov_helper.readCovReports(
        args.compListReport,
        args.allDefectsReport,
        args.ccmReport,
        args.reportName,
        args.swVersion,
    )
