"""
Send metrics to Platform Health for visualization.

This class wraps common methods into an easier to use, standard way of
tagging data and information for Platform Health.

"""

from psycopg2 import connect
from psycopg2.extras import Json
from datetime import datetime
import time


class platformHealthStats:
    """
    Send stats to PostgreSQL for platform health visualization.
    """

    def __init__(self, commitid, project, branch=None, tag=None):
        """
        Initialize the connection with PostgreSQL and provide a standard branch to tag data with for this session.

        Args:
            db_config: Dictionary with database connection details.
            branch: A branch name to tag the metrics with. Default=None
        """
        self.db_config = {
            "dbname": "monitoring",
            "user": "u_wrsd_rw",
            "password": "nMKzMqt8e(Wn",
            "host": "monitoring-wrds.c9qq8e0oklwk.eu-central-1.rds.amazonaws.com",
            "port": "5432",
        }

        self.commitid = commitid
        self.tag = tag
        self.project = project
        self.branch = branch
        self.connection = connect(**self.db_config)
        self.cursor = self.connection.cursor()

    def sendMetric(self, metric_name, metric_value, tags=None):
        """
        Send a Metric to PostgreSQL.

        Args:
            metric_name: Name of the metric to send.
            metric_value: Value of the metric.
            tags: Tags to apply to the metric - used for filtering. Default=None
        """
        if tags is None:
            tags = {}
        else:
            assert isinstance(tags, dict)

        currentDateTime = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        metric_tags = Json(tags)

        # Insert into perception_metrics table
        self.cursor.execute(
            """
            INSERT INTO wrsd."perception_metrics" ("CommitId", date, branch, tag, project, metric_name, metric_value, metric_tags, wrsd_id)
            VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s);
        """,
            (
                self.commitid,
                currentDateTime,
                self.branch,
                self.tag,
                self.project,
                metric_name,
                metric_value,
                metric_tags,
                None,  # WRSD id is for WRSD pipeline information
            ),
        )
        self.connection.commit()

        # print("metric_name =", metric_name)
        # print("metric_value =", metric_value)
        # print("tags =", tags)

    def __del__(self):
        if self.connection:
            self.cursor.close()
            self.connection.close()


if __name__ == "__main__":

    i = 0
    platformHealthInst = platformHealthStats("1234", "Core_radar_gen7v1", "dev")
    while i < 1:
        i += 1
        platformHealthInst.sendMetric("example_metric.gauge", i, tags={"type": "dev"})
        print("i =", i)
        time.sleep(5)
