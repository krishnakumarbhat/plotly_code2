"""
Send metrics to Data Dog for visualization using the datadog module.

This class wraps common datadog methods into an easier to use, standard way of
tagging data and information for datadog.

It also defaults to the Aptiv host and port used in the UniCI CloudBees Jenkisn server.

The module assumes a DogStasD server is running somewhere (CloudBees) to handle the requests
and transport them to DataDog.

See https://docs.datadoghq.com/metrics/ for details on DataDog metrics

"""

from datadog import initialize, statsd
import time


class dataDogStats:
    """
    Send stats to Datadog for visualization.
    """

    def __init__(self, statsd_host="172.24.204.248", statsd_port=8125, branch=None):
        """
        Initialize the connection with DataDog and provide a standard branch to tag data with for this session.

        The default parameters will connect with the DogStatsD Server running on the UniCI CloudBeees instance.

        Args:
            statsd_host: Host IP address of DogStatsD server. Default=172.24.204.248
            statsd_port: Port of the DogStatsD server. Default=8125
            branch: A branch name to tag the metrics with. Default=None
        """
        self.branch = branch
        self.branchTag = []
        if branch:
            self.branchTag = ["branch:" + branch]
        self.options = {}

        self.options.update({"statsd_host": statsd_host})
        print("statsd_host = ", statsd_host)

        self.options.update({"statsd_port": statsd_port})
        print("statsd_port = ", statsd_port)

        initialize(**self.options)

    def sendGaugeMetric(self, metricName, metricValue, tags=None):
        """
        Send a Ggauge Metric to DataDog.

        See https://docs.datadoghq.com/metrics/types/?tab=gauge#metric-types for details on the gauge metric.

        Args:
            metricName: Name of the metric to send to DataDog (See https://docs.datadoghq.com/metrics/custom_metrics/#naming-custom-metrics)
            metricValue: Value of the metric
            tags: Tags to apply to the metric - used for filtering in DataDog. Default=None
        """
        if tags is None:
            tags = []
        else:
            assert isinstance(tags, list)
        tags = tags + self.branchTag
        print("metricName = ", metricName)
        print("metricValue = ", metricValue)
        print("tags = ", tags)
        statsd.gauge(metricName, metricValue, tags=tags)


if __name__ == "__main__":
    i = 20
    dataDogInst = dataDogStats()
    while i < 30:
        i += 1
        retValue = dataDogInst.sendGaugeMetric(
            "luke_example_metric.gauge", i, tags=["environment:dev"]
        )
        print(retValue)
        print("i = ", i)
        time.sleep(5)
