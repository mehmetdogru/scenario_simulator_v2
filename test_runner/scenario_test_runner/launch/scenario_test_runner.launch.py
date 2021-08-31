#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright 2020 Tier IV, Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription

from launch.actions import DeclareLaunchArgument, OpaqueFunction, Shutdown

from launch.conditions import IfCondition

from launch.substitutions import LaunchConfiguration

from launch_ros.actions import Node, LifecycleNode

from pathlib import Path


def architecture_types():
    return ["awf/auto", "tier4/proposal"]


def default_autoware_launch_package_of(architecture_type):
    if architecture_type not in architecture_types():
        raise KeyError(
            f"architecture_type = {architecture_type.perform(context)} is not supported. Choose one of {architecture_types()}."
        )
    return {
        "awf/auto": "scenario_test_runner_launch",
        "tier4/proposal": "autoware_launch",
    }[architecture_type]


def default_autoware_launch_file_of(architecture_type):
    if architecture_type not in architecture_types():
        raise KeyError(
            f"architecture_type = {architecture_type.perform(context)} is not supported. Choose one of {architecture_types()}."
        )
    return {
        "awf/auto": "autoware_auto.launch.py",
        "tier4/proposal": "planning_simulator.launch.xml",
    }[architecture_type]


def launch_setup(context, *args, **kwargs):
    # fmt: off
    architecture_type       = LaunchConfiguration("architecture_type",       default="tier4/proposal")
    autoware_launch_file    = LaunchConfiguration("autoware_launch_file",    default=default_autoware_launch_file_of(architecture_type.perform(context)))
    autoware_launch_package = LaunchConfiguration("autoware_launch_package", default=default_autoware_launch_package_of(architecture_type.perform(context)))
    global_frame_rate       = LaunchConfiguration("global_frame_rate",       default=30.0)
    global_real_time_factor = LaunchConfiguration("global_real_time_factor", default=1.0)
    global_timeout          = LaunchConfiguration("global_timeout",          default=180)
    output_directory        = LaunchConfiguration("output-directory",        default=Path("/tmp"))
    scenario                = LaunchConfiguration("scenario",                default=Path("/dev/null"))
    sensor_model            = LaunchConfiguration("sensor_model",            default="")
    vehicle_model           = LaunchConfiguration("vehicle_model",           default="")
    with_rviz               = LaunchConfiguration("with-rviz",               default=False)
    workflow                = LaunchConfiguration("workflow",                default=Path("/dev/null"))
    # fmt: on

    port = 8080

    def make_parameters():
        print(f"architecture_type       = {architecture_type.perform(context)}")
        print(f"autoware_launch_file    = {autoware_launch_file.perform(context)}")
        print(f"autoware_launch_package = {autoware_launch_package.perform(context)}")
        print(f"vehicle_model           = {vehicle_model.perform(context)}")

        parameters = [
            {"autoware_launch_file": autoware_launch_file},
            {"autoware_launch_package": autoware_launch_package},
            {"architecture_type": architecture_type},
            {"port": port},
            {"sensor_model": sensor_model},
            {"vehicle_model": vehicle_model},
        ]

        if vehicle_model.perform(context):
            parameters.append(
                get_package_share_directory(
                    vehicle_model.perform(context) + "_description"
                )
                + "/config/vehicle_info.param.yaml"
            )
            parameters.append(
                get_package_share_directory(
                    vehicle_model.perform(context) + "_description"
                )
                + "/config/simulator_model.param.yaml"
            )

        return parameters

    return [
        DeclareLaunchArgument("architecture_type", default_value=architecture_type),
        DeclareLaunchArgument(
            "autoware_launch_file", default_value=autoware_launch_file
        ),
        DeclareLaunchArgument(
            "autoware_launch_package", default_value=autoware_launch_package
        ),
        DeclareLaunchArgument("global_frame_rate", default_value=global_frame_rate),
        DeclareLaunchArgument(
            "global_real_time_factor",
            default_value=global_real_time_factor,
            description="Specify the ratio of simulation time to real time. If "
            "you set a value greater than 1, the simulation will be faster "
            "than in reality, and if you set a value less than 1, the "
            "simulation will be slower than in reality.",
        ),
        DeclareLaunchArgument(
            "global_timeout",
            default_value=global_timeout,
            description="Specify the simulation time limit. This time limit is "
            "independent of the simulation playback speed determined by the "
            "option real_time_factor. It also has nothing to do with "
            "SimulationTimeCondition in OpenSCENARIO format.",
        ),
        DeclareLaunchArgument(
            "output-directory",
            default_value=output_directory,
            description="Specify the output destination directory of the "
            "generated file including the result file.",
        ),
        DeclareLaunchArgument(
            "with_rviz",
            default_value=with_rviz,
            description="if true, launch Autoware with given rviz configuration.",
        ),
        DeclareLaunchArgument(
            "scenario",
            default_value=scenario,
            description="Specify a scenario file (.yaml or .xosc) you want to "
            "execute. If a workflow file is also specified by the --workflow "
            "option at the same time, this option takes precedence (that is, "
            "only one scenario passed to the --scenario option will be executed"
            ").",
        ),
        DeclareLaunchArgument("sensor_model", default_value=sensor_model),
        DeclareLaunchArgument("vehicle_model", default_value=vehicle_model),
        DeclareLaunchArgument(
            "workflow",
            default_value=workflow,
            description="Specify a workflow file (.yaml) you want to execute.",
        ),
        Node(
            package="scenario_test_runner",
            executable="scenario_test_runner",
            namespace="simulation",
            name="scenario_test_runner",
            output="screen",
            on_exit=Shutdown(),
            arguments=[
                "--global-frame-rate",
                global_frame_rate,
                "--global-real-time-factor",
                global_real_time_factor,
                "--global-timeout",
                global_timeout,
                "--output-directory",
                output_directory,
                "--scenario",
                scenario,
                "--workflow",
                workflow,
            ],
        ),
        Node(
            package="simple_sensor_simulator",
            executable="simple_sensor_simulator_node",
            namespace="simulation",
            name="simple_sensor_simulator",
            output="screen",
            parameters=[{"port": port}],
        ),
        LifecycleNode(
            package="openscenario_interpreter",
            executable="openscenario_interpreter_node",
            namespace="simulation",
            name="openscenario_interpreter",
            output="screen",
            parameters=make_parameters(),
        ),
        Node(
            package="openscenario_visualization",
            executable="openscenario_visualization_node",
            namespace="simulation",
            name="openscenario_visualizer",
            output="screen",
        ),
        Node(
            package="rviz2",
            executable="rviz2",
            name="rviz2",
            output={"stderr": "log", "stdout": "log"},
            condition=IfCondition(with_rviz),
            arguments=[
                "-d",
                str(
                    Path(get_package_share_directory("scenario_test_runner"))
                    / "config/scenario_simulator_v2.rviz"
                ),
            ],
        ),
    ]


def generate_launch_description():
    return LaunchDescription([OpaqueFunction(function=launch_setup)])
