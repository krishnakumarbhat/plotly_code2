import plotly.graph_objects as go
import logging
import gc
import time
import pandas as pd
from typing import List


class PlotlyCharts:
    """Factory class for creating various types of plots.
    Implements the Factory Method pattern.
    Each plot creation method follows Single Responsibility Principle.
    """

    @staticmethod
    def scatter_plot(
        input_in_linear,
        first_ouput_in_linear,
        second_ouput_in_liner=None,
        signal_name=None,
        first_data_name=None,
        second_data_name=None,
        color_for_first="blue",
        color_for_second="red",
        name_of_plot=None,
        optimize_performance=True,
    ):
        """Create scatter plot comparing two data series.

        Args:
            input_in_linear: X-axis values (usually scan indices)
            first_ouput_in_linear: First series Y values
            second_ouput_in_liner: Optional second series Y values
            signal_name: Name of the signal for plot title
            first_data_name: Label for first data series
            second_data_name: Label for second data series
            color_for_first: Color for first series
            color_for_second: Color for second series
            name_of_plot: Custom name for the plot

        Returns:
            go.Figure: A plotly scatter plot figure
        """
        start_time = time.time()

        # Input validation
        if input_in_linear is None or first_ouput_in_linear is None:
            logging.error(f"Missing required data for scatter plot of {signal_name}")
            return go.Figure()

        # Category configuration
        categories = {
            f"{first_data_name}": {
                "name": f"{first_data_name}",
                "color": color_for_first,
                "symbol": "circle",
            },
            f"{second_data_name}": {
                "name": f"{second_data_name}",
                "color": color_for_second,
                "symbol": "square",
            },
        }

        fig = go.Figure()

        fig.add_trace(
            go.Scattergl(
                x=input_in_linear,
                y=first_ouput_in_linear,
                mode="markers",
                name=categories[first_data_name]["name"],
                marker=dict(
                    color=categories[first_data_name]["color"],
                    symbol=categories[first_data_name]["symbol"],
                    size=2,
                    opacity=0.7,
                ),
            )
        )
        if second_ouput_in_liner is not None:
            fig.add_trace(
                go.Scattergl(
                    x=input_in_linear,
                    y=second_ouput_in_liner,
                    mode="markers",
                    name=categories[second_data_name]["name"],
                    marker=dict(
                        color=categories[second_data_name]["color"],
                        symbol=categories[second_data_name]["symbol"],
                        size=2,
                        opacity=0.7,
                    ),
                )
            )

        gc.collect()

        # Layout configuration

        fig.update_layout(
            title=f"Scatter {name_of_plot} Plot of {signal_name}",
            xaxis_title="Scan Index",
            yaxis_title=f"Values of {signal_name}",
            legend_title="Categories",
            hovermode="closest",
        )
        end_time_scatter = time.time()
        logging.debug(
            f"time taken by scatter api as whole of {end_time_scatter - start_time} in {signal_name}"
        )
        return fig

    @staticmethod
    def bar_plots(
        labels: list,
        title: str = "Composite Analysis",
        label1: str = "Match",
        label2: str = "Mismatch",
        xaxis_title: str = "Signals",
        yaxis_title: str = "Count",
        **kwargs,
    ) -> "go.Figure":
        """
        Generates composite bar chart with customizable bar names and axis labels.

        Accepts either:
        - match_values & mismatch_values
        - input_detection & output_detection

        Args:
            labels: List of signal names
            title: Chart title
            label1: Name for the first bar (default 'Match')
            label2: Name for the second bar (default 'Mismatch')
            xaxis_title: X-axis label
            yaxis_title: Y-axis label
            kwargs: match_values, mismatch_values, input_detection, output_detection

        Returns:
            go.Figure: A plotly bar chart figure
        """
        # Flexible input mapping
        if "match_values" in kwargs and "mismatch_values" in kwargs:
            values1 = kwargs["match_values"]
            values2 = kwargs["mismatch_values"]
        elif "input_detection" in kwargs and "output_detection" in kwargs:
            values1 = kwargs["input_detection"]
            values2 = kwargs["output_detection"]
        else:
            raise ValueError(
                "Provide either (match_values & mismatch_values) or (input_detection & output_detection)"
            )

        if "match_values" in kwargs and "mismatch_values" in kwargs:
            values1 = kwargs["match_values"]
            values2 = kwargs["mismatch_values"]
            # Show numbers for match, percentages for mismatch
            total_values = [v1 + v2 for v1, v2 in zip(values1, values2)]
            match_text = [
                f"{v1} ({(v1 / total * 100):.1f}%)" if total > 0 else "0 (0%)"
                for v1, total in zip(values2, total_values)
            ]
            mismatch_text = [
                f"{v2} ({(v2 / total * 100):.1f}%)" if total > 0 else "0 (0%)"
                for v2, total in zip(values2, total_values)
            ]
        elif "input_detection" in kwargs and "output_detection" in kwargs:
            values1 = kwargs["input_detection"]
            values2 = kwargs["output_detection"]
            # Show numbers for both
            match_text = [str(v1) for v1 in values1]
            mismatch_text = [str(v2) for v2 in values2]
        else:
            raise ValueError(
                "Provide either (match_values & mismatch_values) or (input_detection & output_detection)"
            )

        fig = go.Figure()

        fig.add_trace(
            go.Bar(
                x=labels,
                y=values1,
                name=label1,
                marker_color="green",
                text=match_text,
                textposition="auto",
                hovertemplate=f"{label1}: %{{match_text}} <extra></extra>",
            )
        )

        fig.add_trace(
            go.Bar(
                x=labels,
                y=values2,
                name=label2,
                marker_color="red",
                text=mismatch_text,
                textposition="auto",
                hovertemplate=f"{label2}: %{{text}} <extra></extra>",
            )
        )

        fig.update_layout(
            title=title,
            barmode="group",
            yaxis_title=yaxis_title,
            xaxis_title=xaxis_title,
            height=600,
            width=1000,
            hovermode="x unified",
        )

        return fig

    @staticmethod
    def histogram_with_count(
        input_values: List[float],
        output_values: List[float],
        signal_name: str,
        sensor_position: str = "",
    ) -> go.Figure:
        """Create a histogram comparing input and output value distributions.

        Args:
            input_values: List of input data values
            output_values: List of output data values
            signal_name: Name of the signal for plot title
            sensor_position: Optional sensor position identifier

        Returns:
            go.Figure: A plotly histogram figure
        """
        import plotly.express as px

        if input_values is None and output_values is None:
            logging.error(f"Missing data for histogram of {signal_name}")
            return go.Figure()
        input_values = [] if input_values is None else input_values
        output_values = [] if output_values is None else output_values
        if len(input_values) == 0 and len(output_values) == 0:
            fig = go.Figure()
            fig.update_layout(
                title=f"Histogram of {signal_name} (no data)",
                xaxis_title="Value",
                yaxis_title="Probability Density",
                height=600,
                width=1000,
            )
            return fig

        # Create DataFrame with combined data
        df = pd.DataFrame(
            {
                "Values": pd.concat(
                    [pd.Series(input_values), pd.Series(output_values)]
                ),
                "Type": ["Input"] * len(input_values) + ["Output"] * len(output_values),
            }
        )

        # Create histogram with probability density normalization
        fig = px.histogram(
            df,
            x="Values",
            color="Type",
            nbins=30,
            histnorm="probability density",
            color_discrete_map={"Input": "blue", "Output": "red"},
            opacity=0.6,
            barmode="overlay",
        )

        fig.update_layout(
            title=f"Histogram of {signal_name}",
            xaxis_title="Value",
            yaxis_title="Probability Density",
            height=600,
            width=1000,
        )

        return fig

    @staticmethod
    def create_box(y, title, y_label, color, legend_label):
        """
        Create a Plotly box plot.

        Parameters:
        - x: List of x-axis values (scan indices)
        - y: List of y-axis values (data values)
        - z: List of z-axis values (not used in this case)
        - title: Title of the plot
        - x_label: Label for the x-axis
        - y_label: Label for the y-axis
        - color: Color of the box plot
        - legend_label: Label for the legend

        Returns:
        - fig: Plotly figure object
        """
        fig = go.Figure()
        fig.add_trace(go.Box(y=y, marker_color=color, name=legend_label))

        fig.update_layout(title=title, yaxis_title=y_label, boxmode="group")

        return fig
