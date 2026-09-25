# Studio Pipeline Manager YAML Language Reference 25.05 (Markdown Conversion)

<!-- Page 1 -->

STU DIO PIPELINE  MANA GER YAML LANG UAGE REFERENC E, 25.05

<!-- Page 2 -->

Copyright Notice
Copyright © 2026 W ind Riv er Systems, Inc.
All rights reserv ed. No part of this publication may be reproduced or transmitted in any form or by any means without the prior written permission of W ind Riv er
Systems, Inc.
Wind Riv er, Tornado, and VxWorks are registered trademarks of W ind Riv er Systems, Inc. Helix, Pulsar, Rocket, Titanium Cloud, Titanium Control, Titanium Core,
Titanium Edge, Titanium Edge SX, Titanium Serv er, and the W ind Riv er logo are trademarks of W ind Riv er Systems, Inc. Any third-party trademarks referenced are the
property of their respectiv e owners. For further information regarding W ind Riv er trademarks, please see:
www.windriv er.com/company/terms/trademark.html
This product may include softw are licensed to W ind Riv er by third parties. Relev ant notices (if any) are provided for your product on the W ind Riv er download and
installation portal:
https://gallery.windriv er.com/
Wind Riv er may refer to third-party documentation by listing publications or providing links to third-party w ebsites for informational purposes. W ind Riv er accepts no
responsibility for the information provided in such third-party documentation.
Corporate Headquarters
Wind Riv er
1277 Treat Blvd Suite 700
Walnut Creek, CA 94597
U.S.A.
Toll free (U.S.A.): +1-800-545-WIND
Telephone: +1-510-748-4100
Facsimile: +1-510-749-2010
For additional contact information, see the W ind Riv er w ebsite:
www.windriv er.com
For information on how to contact Customer Support, see:
www.windriv er.com/support
Studio Pipeline Manager Y AML Language Reference, 25.05
17 February 2026

<!-- Page 3 -->

Contents
Studio Pipeline Manager YAML  Language Reference, 25.05 1
Glossary
Pipeline YAML Language Reference
Task YAML Language Reference
Examples

<!-- Page 4 -->

1
Studio Pipeline Manager YAML Language Reference, 25.05
Wind Riv er Studio is a cloud based DevSecOps system used to build and deploy your VxWorks and W ind Riv er Linux dev elopment projects. W ind Riv er
Studio Pipeline Manager is where users can automate dev elopment, test, and deployment tasks. This is a Y AML reference to assist dev elopers and
administrators in creating pipelines.
Please see Wind Riv er Studio  documentation for additional information.

<!-- Page 5 -->

Glossary
-  UI ﬁelds
Pipeline Y AML Language Reference
-  Overview
-  Parameter
-  Parameter Types
-  Pipeline Node
-  Pipeline Super T ask
-  Pipeline Compute
-  Pipeline Storage
-  Pipeline Workspace
-  Pipeline Container Settings
-  Pipeline T ask
-  Final T asks
-  Unstable T asks
Task Y AML Language Reference
-  Task Speciﬁcation (taskSpec)
-  Steps
-  Plain Step Speciﬁcation
-  Microtask Step Speciﬁcation
-  Parameter Speciﬁcation
-  Alternativ e defaults
-  String P arameter Speciﬁcation (stringP aramSpec)
-  Select P arameter Speciﬁcation (selectP aramSpec)
-  Secret P arameter Speciﬁcation
-  Secret P arameter References
-  Aggregate P arameter Speciﬁcation (map / object / array param)
-  Object Properties (propertySpec)
-  Aggregate parameter references
-  Aggregate P arameter Item Render Language
-  Aggregate P arameter T emplate Speciﬁcation (templateSpec)
-  Predeﬁned T emplates
-  Aggregate P arameter Array T emplate Speciﬁcation (arrayT emplateSpec)
-  Aggregate P arameter Env Speciﬁcation (aggrEnvSpec)
-  Resource Speciﬁcation
-  Result speciﬁcation
-  Conditional Steps (stepWhenExpr )
-  Variable Substitution
Examples
-  Pipeline
-  Task
-  Pipeline Example with Final T ask
-  Pipeline Example with Compute
-  Pipeline Example with P ersistent Storage
-  Pipeline Example with Container Settings - Simple T ask
-  Pipeline Example with Container Settings - Complex T ask (use of microtask concept)
-  Pipeline Example with Pipeline Workspace
-  Pipeline Example with Timeouts
-  Pipeline Example with Conditional T asks
-  Task Example with Conditional Steps
-  Task Example using Unstable State
Glossary
Term Description
Tekton Kubernetes nativ e pipeline system, used under the hood by the WR Studio
Pipeline system. Not directly exposed to end users
Pipeline A graph of T asks describing a re-usable automation job
Task A re-usable “function” that takes inputs (params and resources), produces
outputs (results), and deﬁnes an action as a series of steps
PipelineT ask An instance of a T ask in a Pipeline
Step An unit of work inside a T ask. Can be a simple step, or a Microtask step

<!-- Page 6 -->

Term Description
Microtask A step that contains another T ask.
Resource An abstraction for anything that is access controlled, e.g. a Git repo, or an
artifact bucket
Vault Service that provides secret management for Studio Dev eloper
Vault Policy Policy in V ault giving access to a speciﬁc set of secrets. Ev ery Pipeline is
imbued with a V aultPolicy.
UI fields
Names deﬁned in this ﬁle should be v alid T ekton Names. If the UI w ants to support arbitrary “display names” (e.g “My Fancy P arameter”), it is
recommended that a diﬀerent uiName ﬁeld be added to support that (initial v alue should be any provided “name”). This ﬁeld is optional and only used by
the UI.
Pipeline YAML  Language Reference
Overview
langVersion: v1
name: name
description: description
ui:
name: Display Name On UI
meta:
locked: false
params:
- param
- param
env:
envName: envValue
config:
name: value
tasks:
- pipelineNode
- pipelineNode
final:
- <taskRef>
- <taskRef>
defineTasks:
- <task>
- <task>
timeout: <timeout>
compute:
- <compute>
- <compute>
storage:
- <storage>
- <storage>
workspace:
<pipelineWorkspace>
containerSettings:
- <containerSetting>
- <containerSetting>

<!-- Page 7 -->

Property Required Default Description
langV ersion N latest v ersion WindRiv er Pipeline Language
version used by this ﬁle
name Y Pipeline Name
description N Pipeline description
ui.name N name Display Name for Pipeline in UI
params N Array of pipeline parameters
env N Environment v ariables to set in every
step container (ov erriden by any
task/step speciﬁc settings)
conﬁg N Set “global” v ariables that can be
used by any task in this pipeline via
$(config.name)
tasks Y Array of pipeline nodes (tasks or
super tasks)
ﬁnal N Array of tasks to be executed in
parallel at the end of the pipeline run
- see Final T asks
deﬁneT asks N Array of task deﬁnitions. If provided,
these will be treated as an ov erlay
over the task library. See Task
meta N Admin ﬁelds can only be changed by
users with the “lead” role in the
pipeline
meta.locked N false Boolean. If true, then pipeline can
only be changed by a user with the
“lead” role in this pipeline
timeout N 8h Maximum duration of this pipeline,
e.g. “2h” - for format details, see
https://pkg.go.dev/time#P arseDuration
compute N Array of named compute sets for use
by pipeline tasks - see Pipeline
Compute
storage N Array of named storage volumes for
use by pipeline tasks - see Pipeline
Storage
workspace N Conﬁguration v alues of the pipeline
workspace to be used by pipeline
tasks - see Pipeline Workspace
containerSettings N Array of named container settings for
use by pipeline tasks - see Pipeline
Container Settings

<!-- Page 8 -->

Parameter
params:
- name: name
description: description
type: <ParamType>
default: <defaultValue>
Property Required Default Description Constraints
name Y Parameter name Valid T ekton Name
description N Parameter description
type N string Parameter Type “string”, “select”, “secret”,
“map”, “array”, or custom
value
default N Default v alue The format of the v alue
should match the parame
type
Parameter T ypes
There are sev eral kinds of parameter, distinguished by the “type” ﬁeld. If the “type” ﬁeld is empty or not one of the recognized v alues, the parameter is
treated as a “string” parameter.
Type Description Details
string A string See String P arameter Speciﬁcation
(stringP aramSpec)
select A string chosen from a list of options See Select P arameter Speciﬁcation
(selectP aramSpec)
secret Reference to secret in v ault See Secret P arameter V alue Speciﬁcation
(secretP aramV alue)
map Set of Name/V alue pairs See Map P arameter V alue Speciﬁcation
(mapP aramV alue)
object Set of Name/V alue pairs with ﬁxed names See Object P arameter V alue Speciﬁcation
(objectP aramV alue)
array Sequence of V alues See Array P arameter V alue Speciﬁcation
(arrayP aramV alue)
Pipeline Node
The pipeline is built up of a tree of nodes. A node is a Pipeline T ask (single task) or a Pipeline Super T ask (array of nodes).
Pipeline Super T ask
Super T asks are an array of nodes running in series or in parallel.
Sequential
tasks:
- node
- node
Parallel
parallelTasks:
- node

<!-- Page 9 -->

- node
Pipeline Compute
compute:
- name: large
resource: largeComputeSetWrrn
- ...
- resource: defaultComputeSet
An array of named compute resources. To use a compute resource, the pipeline bot must hav e view er role or higher in that resource.
At most one unnamed compute resource can be provided. This will be used as the default compute resource for any pipeline task which does not explicitly
request a named compute resource. If no unnamed compute resource is speciﬁed, a Studio default compute speciﬁcation is used.
Pipeline Storage
storage:
- name: <storageName>
resource: storageWrrn
readOnly: true|false
mountPath: <mountPath>
defaultMount: true|false
- ...
- <storage>
Property Required Default Description Constraints
name Y Name to use for this storage Must be unique within this
block
resource Y WRRN or name of resource
which deﬁnes a storage
volumeMust be accessible to the
pipeline bot
mountP ath N Path at which to mount this
volumeOptional. Can be ov erriden at
the pipeline task lev el
readOnly N false true if volume should be
mounted as read-only
defaultMount N false if true then this volume will
be automatically mounted in
every pipeline task
Pipeline W orkspace
A Pipeline workspace is a w ay for a user to deﬁne a volume to share data among tasks. The main functionally of a Pipeline Workspace is to let the Pipeline
Tasks to share context by allowing them to read/write to the same storage letting PLM to ensure that all tasks using the workspace will run in the same node
with the help of Aﬃnity Assistant.
Two important diﬀerences betw een a Pipeline workspace and persistent storage are:
1. With a Pipeline workspace, PLM will ensure that all tasks that are using the workspace will be scheduled in the same node.
2. A Pipeline workspace can use a P ersistent Storage or can create a temporary PVC. For the case of the temporary PVC, it will be mounted to the tasks and
will be destroy ed some minutes after the run ﬁnished.
workspace:
name: <workspaceName>
template:
size: size
accessMode: accessMode
storage: <storageName>
compute: <computeName>
defaultMount: true|false
subPath: <mountPath>
readOnly: true|false

<!-- Page 10 -->

Property Required Default Description Constraints
name Y It is the name to identify and
reference the workspace.
template N The storage template is used
to deﬁne how the temporal
PVC will be created. Here the
user can deﬁne the size and
the access mode for the
temporary PVC.Mutually exclusiv e with
storage
template.size N It is the size for the
temporary PVC, for example
10Gi, 500Mi, 80Gi
template.accessMode N It is the access mode to set up
the modes supported by that
particular volume. For
example: ReadWriteOnce,
ReadWriteMany,
ReadOnlyMany,
ReadWriteOncePod.
storage N Name of the storage (already
set in the pipeline) to use in
the workspaceMutually exclusiv e with
template
compute N Name of the compute
(already set in the pipeline)
to use in the workspace
defaultMount N false This is a boolean v alue that
indicates if the workspace
will be mounted in all task by
default or not.
subP ath N This is a string v alue used to
deﬁne a subP ath directory in
the volume mounted.
readOnly N false This is a boolean v alue used
to deﬁne if the volume
mounted in the workspace
will be read only.
Workspace syntax
There are multiple commands av ailable related to pipeline workspace, the commands are described in the following table:
Syntax Description
$(workspace.<name>.path) Speciﬁes the path to a Workspace where is the name of the Workspace. This
will be an empty string when a Workspace is declared optional and not
provided by a T askRun. Output example: /workspace/my-workspace.
$(workspace.<name>.bound) Either true or false, speciﬁes whether a workspace w as bound. Alw ays true
if the workspace is required. Output example: true.
$(workspace.<name>.claim) Speciﬁes the name of the P ersistentVolumeClaim used as a volume source
for the Workspace where is the name of the Workspace. If a volume source
other than P ersistentVolumeClaim is used, an empty string is returned.
Output example: ‘pvc-5844cd937a’.

<!-- Page 11 -->

Syntax Description
$(workspace.<name>.volume) Speciﬁes the name of the Volume provided for a Workspace where is the
name of the Workspace. Output example: ‘ws-f5d1a’.
Common Pipeline W orkspace validations
1. Pipeline Lev el
Workspace name must be deﬁned.
Either a template or storage must be conﬁgured in workspace, but no both.
When a template is deﬁne, the user must speciﬁed the size and the accessMode.
2. Task Lev el
Workspace name must be deﬁned and conﬁgure at pipeline lev el.
Either workspace or compute must be conﬁgured on the task, but not both.
The storage used in workspace can’t not be set in the task as a separated storage at the same time.
Workspace in task can’t be set as ‘read only = false’, when the workspace is conﬁgure as read only in the pipeline lev el
Pipeline Container Settings
Container settings are a resource that can be used in a pipeline deﬁnition to specify the image, CPU, memory, and ephemeral storage that will be set in a
speciﬁc step (container ) of the pipeline.
The container settings deﬁnition is optional for the steps that are part of the pipeline deﬁnition, for those cases where there isn’t a container settings resource
or the v alue is not deﬁned as part of the speciﬁcation of the resource, the container spec will grab the v alues set in the compute set resource if there is one
deﬁned for the task, if not the default v alues will be set to the container.
Additionally, the container settings resource will ov erride the image if this w as deﬁned in the task Y AML deﬁnition.
containerSettings:
- name: <storageName>
resource: storageWrrn
- ...
Property Required Default Description Constraints
name Y It is the name to identify and
reference the workspace.
resource Y WRRN or name of resource
which deﬁnes a container
setting.Must be accessible to the
pipeline bot
Pipeline T ask
A Pipeline T ask corresponds to a “block” in Pipeline Manager v1. A Pipeline T ask can be thought of as a function call, and has two parts:
task (“function being called”)
parameters to pass to the task (“arguments to the function”)
The task can be a reference to a named library task.
task: <taskRef>
Alternativ ely it can be an inline taskSpec (see Task Speciﬁcation (taskSpec) ):
taskSpec: <taskSpec>
name: name
runAfter:
- pipelineTaskName1
- pipelineTaskName2
task: taskName # either this
taskSpec: <taskSpec> # or this
params:
aStringParam: <stringParamValue>

<!-- Page 12 -->

aSecretParam: <secretParamValue>
aMapParam: <mapParamValue>
anArrayParam: <arrayParamValue>
resources:
resourceName: <resourceValue>
when:
- <whenExpr>
- ...
timeout: <timeout>
retries: <retries>
compute: <computeName>
storage:
- <pipelineTaskStorage>
- <pipelineTaskStorage>
workspace:
name: <name>
mountPath: <mountPath>
subPath: <subPath>
readonly: <readonly>
steps:
- <stepContainerSetting>
- <stepContainerSetting>
Property Required Default Description Constraints
name N Name Valid T ekton name. Unique
within this pipeline.
runAfter N Array of Pipeline T ask names
that this Pipeline T ask must
run after.
runAfter is not intended for
end-user use (although it is
safe for them to do so) since
users can achiev e (almost)
arbitrary ordering constraints
using tasks and
parallelT asks. It is provided
for use internally by the
transpiler “linearize” phase.
task N Name of library T ask Valid T ekton name. Mutually
exclusiv e with taskSpec.
taskSpec N Inline deﬁnition of T ask Mutually exclusiv e with task.
params N Map of parameter
name/v alue pairs. The format
of the v alue depends on the
parameter typeTo leav e a parameter
unspeciﬁed, simply omit it.
Do not deﬁne it to a blank
value since that is treated as
explicitly setting it to an
empty string.
resources N Map of resource name /
resource v alue pairs
when N Array of conditions to check
before executing this Pipeline
Task. See Conditional T asks
timeout N No timeout Maximum duration of this
task, e.g. “2h”For format details, see
https://pkg.go.dev/time#P arseDuration

<!-- Page 13 -->

Property Required Default Description Constraints
retries N 0, don’t retry Number of times to retry this
task if it fails
compute N Name of compute to be used
to run this pipeline task
storage N Array of named storage
volumes used by this
pipeline task - see Pipeline
Task Storage
workspace N Name of workspace to be
used by this pipeline task -
see Pipeline T ask Workspace
steps N Array of steps to set the
container setting - see
Pipeline T ask Container
Setting
Task References
A task can be referenced from a Pipeline T ask (“main task”), or from a step (“microtask”). In both cases, the reference has the following format:
task: [category/]name[@version]
Notice that [] denotes an optional component. The syntax allows “category” to be a path including slashes (“/”), although the initial set of allow ed
categories do not take adv antage of that feature.
Categories are purely an organizational conv enience for human users. T asks work the same w ay regardless of what category they are in.
WindRiv er deﬁned tasks will not use the “misc” category; customer deﬁned tasks can use any category.
Available categories:
Category
scan
build
assemble
test
deploy
utils
misc
Examples:
Specific version of “vxworks” task in “build” category
task: build/vxworks@1.0.0
Latest version of “vxworks” task in “build” category
task: build/vxworks

<!-- Page 14 -->

Latest version of “vxworks” task in any category
task: vxworks
Parameter V alues
Secret Parameter V alue Specification (secretParamV alue)
Secret parameters (and their defaults) are typically set from pipeline or resource secrets:
params:
mySecret: $(resources.<resourceName>.secrets.<secretName>)
or pipeline secrets:
params:
mySecret: $(secrets.<secretName>)
Map Parameter V alue Specification (mapParamV alue)
params:
aMapParam:
key1: "value1"
key2: "value2"
The keys represented by a map parameter are not constrained in any w ay, so code that uses mapP arams must be able to handle arbitrary keys.
Object Parameter V alue Specification (objectParamV alue)
params:
anObjectParam:
key1: "value1"
key2: "value2"
The keys represented by an object parameter are constrained by the properties  ﬁeld ( Object Properties (propertySpec) ).
Array Parameter V alue Specification (arrayParamV alue)
params:
anArrayParam:
- value1
- value2
Resource V alue (resourceV alue)
A resource is an object containing information that is needed to access a speciﬁc access-controlled resource in a tool.
component.endpoint : name
resource.project : https://tool.cloud
secrets.password : $(secrets.tool-password)
Typically, instead of providing a full object, a W ind Riv er Resource Name is provided as follows:
wrrn : wind-river-resource-name
This tells Pipeline Manager to load the corresponding resource from resource manager.
Object References
Map, object, resource, and secret v alues can be bound directly from another object, including a conﬁg “object”.
params:
myMapParam: $(params.myOuterMapParam)
mySecretParam: $(params.mySecretMapParam)
resources:
myResource: $(resources.myOuterResource)
Alternativ ely, deﬁne a conﬁg “object” by deﬁning sev eral properties with the same preﬁx.
config:
myResource:
component.endpoint: name
resource.project: https://tool.cloud

<!-- Page 15 -->

This is shorthand for:
config:
myResource.component.endpoint: name
myResource.resource.project: https://tool.cloud
You can then bind that object to a map, object, or resource.
resources:
myResource: $(config.myResource)
This is equiv alent to:
resources:
myResource:
component.endpoint: name
resource.project: https://tool.cloud
Object sugar
For conv enience, all objects (conﬁg settings, maps, objects, and resources) can optionally be speciﬁed using any combination of ﬂat and nested structures.
component:
endpoint : name
resource:
project: https://tool.cloud
secrets:
password: $(secrets.tool-password)
This is automatically conv erted to:
component.endpoint : name
resource.project : https://tool.cloud
secrets.password : $(secrets.tool-password)
Pipeline T ask Storage
storage:
- name: storageName
storage: pipelineStorageName
spec: TODO
mountPath: somePath
readOnly: true|false
Property Required Default Description Constraints
name N Value of storage Name to use for this storage
in this pipeline taskMust be unique within this
pipeline task
storage Y Name to pipeline storage
volumeMust be the name of one of
the storage volumes deﬁned
in the pipeline-lev el
“storage” block
mountP ath N mountPath  set by pipeline
storage volumePath at which to mount this
volumeIf not set here or at the
pipeline lev el then defaults to
/volumes/<storageName>
readOnly N false true if volume should be
mounted as read-onlyEven if not set, the volume
will still be mounted as read-
only if readOnly:true  is
speciﬁed at the pipeline lev el
Pipeline T ask W orkspace
workspace:
- name: workspaceName
mountPath: <mountPath>
subPath: <subPath>
readOnly: true|false

<!-- Page 16 -->

Property Required Default Description Constraints
name Y It is the name to identify and
reference the workspace that
was previously declare at the
pipeline lev el.
mountP ath N This is an optional key and it
is used to deﬁne the path
used to mount the volume
for the workspace. For
example ‘/mydata’
subP ath N This is an optional key and it
is used to deﬁne a subP ath
directory in the volume
mounted. For the cases
where a Pipeline declaring a
Workspace with subP ath of
/foo and a Pipeline who
binds it to a T ask with
subP ath of /bar will end up
mounting the Volume’s
/foo/bar directory
readOnly N false This is an optional key and it
is used to deﬁne if the
volume mounted in the
workspace will be read only.
Pipeline T ask Container Setting
steps:
- name: stepName
containerSettings: <containerSettingName>
- name: stepName
steps: # array of steps in the case of a Task using "microtask steps"
- name: stepName
containerSettings: <containerSettingName>
Property Required Description Constraints
name Y Name of the step In case that a step doesn’t hav e a
name, the task should be updated to
include step names.
containerSettings N It is the name to identify and
reference the container setting that
was previously declare at the
pipeline lev el.Mutually exclusiv e with steps. Use it
for steps that are not a microtask
steps N Array of steps to set the container
setting.Mutually exclusiv e with
containerSettings. Use it for steps
that are a microtask
Controlling T ask Execution
There are sev eral mechanisms which can be used to control whether a pipeline task will be executed.
Conditional T asks (whenExpr)
You can specify that a pipeline task should only be run when one or more conditions are satisﬁed:
when:
- input: "<string>"
operator: in|notin
values:
- string1

<!-- Page 17 -->

- string2
- ...
Property Required Default Description Constraints
input Y String to match String, which can include
variable substitutions
operator Y How to perform the match “in” or “notin”
values Y Array of strings to match
againstInput must be present or not
present in this array,
depending on whether
operator is “in” or “notin”
Note that input  and values  can include the following kinds of v ariable substitution:
$(params.paramName)  - refers to a pipeline parameter ( not a parameter of this pipeline task)
$(tasks.aTasks.results.aResult)  - refers to the result of a previous task
$(studio.name) , $(config.name)  - standard substitutions, see documentation on V ariable Substitutions
Example:
when:
- input: $(params.buildMode)
operator: in
values:
- optimize
- debug
- input: $(tasks.build.results.status)
operator: notin
values: ["failed"]
In this case the pipeline task will only run if both of the following conditions hold:
1. The pipeline parameter “buildMode” has one of the v alues “optimize” or “debug”
2. The task named “build” produced a result called “status” with any v alue other than “failed”
Skipping a T ask
During dev elopment it can be conv enient to not hav e to run ev ery task in a pipeline ev ery time you w ant to test a change.
A task can be temporarily disabled using the following syntax:
meta:
disabled: true
Such a task will not be included in the pipeline when it is run. How ever note that if another task depends on the results of the disabled task, an error will be
raised when the pipeline is executed.
For example:
name: my-pipeline-task
task: studio-cli
params:
script: |
echo hello
meta:
disabled: true
Bypassing a T ask
Instead of disabling a task altogether, you can specify results that it should return.
meta:
results:
<resultName>: <resultValue>
...
When the pipeline is run, such a task will be replaced with a v ery simple/fast task that simply produces the speciﬁed results, which can then be used by
later tasks. This allows a task to be eﬀectiv ely skipped, without having to make signiﬁcant changes to the pipeline.

<!-- Page 18 -->

Final T asks
With the inclusion of Final T asks, PLM enables the pipeline dev eloper to be able to add tasks that will be executed when the pipeline ﬁnishes running. This
is helpful in multiple cases, for example: cleaning up, writing a report, or for notiﬁcations.
Final tasks are guaranteed to be executed in parallel after all main tasks execution regardless of success or error.
Final tasks are optional, for the cases when they are needed the user can specify a list of one or more ﬁnal tasks under the ﬁnal section.
Final tasks are independent of the result of main tasks, what this means is that if a main task fail the ﬁnal task is going to be executed as normal, unless, a
condition has been set for the task.
If the pipeline run is cancelled the ﬁnal task are going to be also cancelled and not executed.
Pipeline run status with Final T asks
Main T asks Final T asks Pipeline Run Status
All tasks successful All tasks successful Successful
All tasks successful One or more tasks failure Failure
One or more tasks skipped and rest successful All tasks successful Successful
One or more tasks skipped and rest successful One or more tasks failure Failure
Single failure of task All tasks successful Failure
Single failure of task One or more tasks failure Failure
Unstable T asks
To improv e PLM task and pipeline state management, a new “Unstable” state will be introduced. This state can be programmatically set by the task
developer when a task completes successfully but has issues that require attention, such as vulnerabilities detected or failing tests. This ensures that
pipelines can continue running while ﬂagging tasks for later inv estigation.
Provides better visibility into tasks that complete with w arnings instead of outright failures.
Prev ents unnecessary pipeline failures, allowing non-critical issues to be inv estigated later.
Pipeline run status with unstable state
Main and Final T asks Pipeline Run Status
All tasks successful Successful
One or more tasks unstable and rest successful Unstable
One or more tasks failure Failure
One or more tasks unstable and one or more tasks canceled Canceled
Unstable state modification rules
The “Unstable” state can only be set by the task’s predeﬁned interpretation of its results (deﬁned by the task dev eloper ).
Users cannot manually change a task’s state to “Unstable” from the UI.
Setting a task as “Unstable”
Programmatically, use the predeﬁned result property named “unstable” and set it as true. Y ou can add any “if” condition to set the state as unstable:
name: my-pipeline-task
params:
- name: param
type: string
results:
- name: output
steps:
- name: main
script: |

<!-- Page 19 -->

#!/bin/bash
if [ expression ]; then
echo "true" > $(results.unstable.path)
fi
$(results.unstable.path)  refers to the predeﬁned / new reserv ed result v alue in which the task dev eloper can set it with a “true” string v alue to
change the state of a task if needed.
Task YAML  Language Reference
WindRiv er Studio Pipeline Manager provides its own task library language. Although the language takes inspiration from T ekton, and other pipeline
systems, it is not T ekton. Generally speaking, users need not care that T ekton is used “under the hood”.
Task Specification (taskSpec)
langVersion: v1
name: name
category: build
description: description
ui:
name: Display Name On UI
meta:
locked: false
params:
- paramSpec
- secretParamSpec
- mapParamSpec
- objectParamSpec
- arrayParamSpec
resources:
- resourceSpec
results:
- resultSpec
steps:
- step
- step
Notice that steps can either be user-deﬁned, or references to other library T asks.
Property Required Default Description Constraints
name Y Task name (slug) Valid T ekton Name
category Y Task category build, test, scan, etc
version N Task v ersion e.g. “1.0.0”
description Y Task description
params N Array of parameters
resources N Array of resources
results N Array of results
steps Y Series of steps
meta.locked N false A task with this ﬁeld set can
only be updated by a PLM
Admin, or a member of the
plm-admin-group.

<!-- Page 20 -->

Steps
Steps can be either “plain steps” or “microtask steps”. Microtask steps allow building tasks out of other tasks. W e use the term “microtask” to refer to a T ask
when used in a microtask step.
Plain Step Specification
name: stepName
image: containerImage
command:
- 'cmd'
- 'cmdArg2'
args:
- 'arg1'
- 'arg2'
script: script
env:
envVar1: envValue1
envVar2: envValue2
onError: <errorBehavior>
when: <stepWhenExpr>
Property Required Default Description Constraints
name N Step name (slug) Valid T ekton Name
params N Array of parameters
image Y $(studio.cliImage) Container image for step
command N Container command for step
args N Container args for step
script N Container script for step mutually exclusiv e with
command
env N Map from environment
variable name to v alue
onError N What to do if step fails?
continue or stopAndFail.defaults to stopAndFail
when N Array of conditions to check
before executing this step -
see Conditional Stepsany v ariables appearing in
conditions must be ev aluable
at launch time
onError
Steps indicate failure by returning (exiting with) a non zero exit code. Normally when a step fails, this cascades to a failure of the parent task, and ultimately
the entire pipeline.
Sometimes this default behavior is not desirable. Use the onError  step property to change this behavior. Allow ed v alues are:
Value Description
stopAndFail If this step fails then the entire task fails (default behavior )
continue Continue ev en if this step fails. Whether or not this step succeeds has no
eﬀect on the success of the containing task.
You can also use v ariable references such as $(params.myParam)  in the v alue of onError, with the following limitation: the v alue of onError  must
evaluate to a known string (either “continue” or “stopAndFail”) at the time the pipeline is launched. In particular if the v alue ultimately depends on the
result of an earlier task, then the pipeline will fail to run.
See also: Referencing ExitCode of an earlier step .

<!-- Page 21 -->

Microtask Step Specification
name: stepName
task: taskName # either this
taskSpec: <taskSpec> # or this
params:
paramName1: paramValue1
paramName2: paramValue2
resources:
resourceName1: resourceValue1
resourceName2: resourceValue2
returnResults:
microtaskResultName : parenTaskResultName
when: <stepWhenExpr>
Property Required Default Description Constraints
name N Step name (slug) Valid T ekton Name
task N Reference to library T ask
taskSpec N Inline T ask deﬁnition
params N Map of parameters to pass in
to microtask
resources N Map of resources to pass in
to microtask
returnResults N Map from results of the
microtask to results of the
parent task
when N Array of conditions to check
before executing this stepany v ariables appearing in
conditions must be ev aluable
at launch time
Microtask overview
Note that a PipelineT ask can refer to a task which can hav e sev eral steps, each of which refers to a microtask, each of which has steps, … . To manage this
complexity the following considerations should be taken into account.
Microtasks can take parameters of any kind
Microtasks can produce results. Results can be “forw arded” to a result of the parent task (see Results Forw arding ), or referenced by later steps using
$(steps.<stepNameOrNumber>.results.<resultName>.path) , which ev aluates to the name of a ﬁle holding the result.
Each step is its own “scope”, so there is no ambiguity if a microtasks parameter or result happens to hav e the same name as a parent task or result.
In the following example, some-task  has (at least) two parameters option1  and option2 , which are bound to the v alues of the parent task parameters
option2  and option1  respectiv ely; and (at least) two results result1  and result2 , which are forw arded to the parent task results result2  and
result1  respectiv ely.
step:
- task: some-task
params:
option1: $(params.option2)
option2: $(params.option1)
returnResults:
result1: result2
result2: result1
Results Forwarding
Only the top-lev el task in a pipeline task can return results to pipeline manager. How ever it is possible to “forw ard” a result of a child task to one of the
parent’s result slots.
returnResults:
childResult: parentResult
This will return the child result named “childResult” through the parent’s result named “parentResult”.

<!-- Page 22 -->

Referencing Results of an earlier step
Results of earlier steps can be referenced as follows. In this example it is assumed that the “jenkins” task produces a result named “jenkinsOutput”.
steps:
- name: jenkins-step
task: jenkins
- name: reference-step
script: |
cat $(steps.jenkins-step.results.jenkinsOutput.path)
For conv enience, if a step is unnamed, but references a “task”, you can lookup results using the task name, or any preﬁx of the name, so long as this leads to
a single unambiguous match.
For example:
steps:
- task: jenkins
- name: reference-step
script: |
cat $(steps.jenkins.results.jenkinsOutput.path)
Note that only results of earlier steps can be referenced. In the following example, step “references-step” contains an unambiguous reference to the ﬁrst
jenkins step.
steps:
- task: jenkins
- name: reference-step
script: |
cat $(steps.jenkins.results.jenkinsOutput.path)
- task: jenkins
You can also reference steps by (1-based) index:
steps:
- task: jenkins
- name: reference-step
script: |
cat $(steps.1.results.jenkinsOutput.path)
- task: jenkins
Referencing ExitCode of an earlier step
A ﬁle containing the exit code of an earlier step is referenced as follows.
$(steps.<stepNameOrNumber>.exitCode.path)
For details on how to specify stepNameOrNumber  see the previous section.
Parameter Specification
params:
- name: name
type: artifactPath
description: description
default: default
ui:
name: Display Name
order: display order in ui
hidden: whether to hide this param in ui
Property Required Default Description Constraints
name Y Parameter name Valid T ekton Name
description Y Parameter description

<!-- Page 23 -->

Property Required Default Description Constraints
type N string type of param. Can be string,
lx-project, ect
default N Default v alue (see Alternativ e
defaults )
ui.name N name Display Name for param in
## Ui
ui.order N random Display order of param in UI integer
ui.hidden N true If true then do not show this
param in UIboolean
ui.dependsOn N Name of another param
ui.multiline N If true shows a textarea type
component in the UI. This is
only supported for type
string componentsboolean
Alternative defaults
To make writing reusable tasks easier, default v alues can be expressed as a list of alternativ es. All but the last alternativ e must be a reference to a conﬁg
variable - $(config.somevar) , or a secret - $(secrets.secretName) . If any of those references ev aluate to a non-empty string, then that is used as the
default, otherwise the last alternativ e is used.
This syntax can be used with regular string parameters, secret parameters, and property defaults for object parameters.
It provides a w ay to provide allow the behavior of a task to be customized within a pipeline by deﬁning a global conﬁguration v ariable, while still
providing a sensible default for users who do not need to customize it.
For example:
params:
- name: mycredentials
type: secret
default: $(config.preferredCredentials) || $(secrets.credentials)
In the abov e example, the secret parameter “mycredentials” usually defaults to the pipeline secret named “credentials”, but this can be ov erriden if the
pipeline deﬁnes the conﬁg v ariable “mycredentials”.
config:
preferredCredentials: $(secrets.someOtherCredentials)
In the abov e example, the parameter will default to the pipeline secret named “someOtherCredentials”.
The alternativ e defaults syntax can end with a “blank” alternativ e to indicate that the parameter is optional. For example:
params:
- name: mysecret
type: secret
default: $(secrets.seckey) ||
In the abov e example, the parameter will default to the pipeline secret named “seckey” if that secret exists, otherwise it will default to empty. In this case
steps which use the secret parameter need to check for the case that it is empty. In that case $(params.mysecret.path)  will ev aluate to an empty string
instead of a ﬁlename.
String Parameter Specification (stringParamSpec)
params:
- name: aSelectParam
type: select
description: description
default: <defaultValue>
env: <envVar>

<!-- Page 24 -->

Property Required Default Description Constraints
default N Default v alue of this
parameter. If not set then the
user must provide a v alue
env N Name of an environment
Select Parameter Specification (selectParamSpec)
A select parameter must hav e one one of the speciﬁed v alues. Note that this is only enforced for v alues that are known at the time the pipeline is executed.
For example, if the v alue is the result of a previous task, like $(tasks.mytask.results.myresult) , then no v alidation is done.
params:
- name: aSelectParam
type: select
description: description
default: <defaultValue>
env: <envVar>
options:
- name: nameToDisplayInUI
value: valueToPassToTask
description: descriptionTOShowInUi
The options  array lists the allow ed set of v alues.
Property Required Default Description Constraints
default N Default v alue of this
parameter. If not set then the
user must provide a v alue
env N Name of an environment
variable to set based on this
parameter
options[].name Y Text to show in UI
options[].description Y Description of this option
options[].v alue Y Value to use if this option is
chosen
Secret Parameter Specification
Note that secret parameters are not currently shown or settable from the UI.
params:
- name: aSecretParam
type: secret
description: description
default: <secretParamValue>
Property Required Default Description Constraints
name Y Parameter name Valid T ekton Name
description Y Parameter description
default N Default location of this secret See Secret P arameter V alue
Speciﬁcation abov e
env N Name of an environment
variable to set based on thisThe v alue of the environment
variable is the name of a ﬁle

<!-- Page 25 -->

Property Required Default Description Constraints
parameter containing the secret, not the
secret itself
Secret Parameter References
Secrets are “rendered” to a ﬁle at runtime, based on the render speciﬁcation. The ﬁle can be referenced as follows.
$(params.aSecretParam.path)
For multi-key secrets (e.g. a credential with keys “username” and “password”) individual keys can be extracted as follows.
$(params.aSecretParam.<key>.path)
Consider the following case.
$(params.aSecretParam.path)
By not including a key, this will produce a ﬁle using a template provided by the secret creator.
Aggregate Parameter Specification (map / object / array param)
params: # or arrayParams
- name: myAggrParam
description: description
properties:
- <propertySpec>
- <propertySpec>
default: <mapParamValue> # or <arrayParamValue>
envTemplate: <aggrEnvSpec>
render:
templateName: "json|map|..."
template: <templateSpec>
argTemplate: <arrayTemplateSpec>
arrayTemplate: <arrayTemplateSpec>
Property Required Default Description Constraints
name Y Parameter name Valid T ekton Name
description Y Parameter description
properties Y Optional list of properties for
object parameter
default N Default v alue for this
aggregate paramSee Map P arameter / Array
Parameter V alue
Speciﬁcation abov e
envT emplate N Template describing how to
generate environment
variables for this param
render.templateName N Name of a predeﬁned
template to use to render the
aggregate to a ﬁleFor allow ed v alues see
below. Mutually exclusiv e
with render.template
render.template N Template deﬁning how to
render the aggregate to a ﬁleMutually exclusiv e with
render.templateName
render.argT emplate N Template deﬁning how to
expand “star” references that
appear in the args or comand
ﬁelds of a step

<!-- Page 26 -->

Property Required Default Description Constraints
render.arrayT emplate N Template deﬁning how to
expand “star” references in
any array context
Object Properties (propertySpec)
By default map parameters can be bound to any set of key, v alue pairs. If you w ant to enforce a particular structure, you can use “properties”.
params:
- name: aMapParam
properties:
- name: code
description: Git repo containing code
ui:
name: Code
order: 1
- name: branch
default: main
- name: path
default: ""
Note that when binding such a parameter, all properties that do not hav e a default are required to be bound. Here are some examples of allow ed and
disallow ed bindings.
# Valid
params:
aMapParam:
code: https://mycode.com/project/1
# Invalid - missing property "code"
params:
aMapParam:
branch: develop
# Invalid - unexpected property "file"
params:
aMapParam:
code: https://mycode.com/project/1
file: foo.c
Property Required Default Description Constraints
name Y Parameter name Valid T ekton Name
description Y Parameter description
default N Default v alue for this
property
ui.name N name Display Name for property
in UI
ui.order N random Display order of property in
UIinteger
Aggregate parameter references
There are four w ays to use aggregate parameters. Y ou are permitted to mix any or all of them in the same task.
Object references
For the common case of passing a copy of an array or map parameter as the v alue of another parameter of the same type, you can use the same syntax you
would use to pass a string parameter:
params:
newParam: $(params.oldParam)

<!-- Page 27 -->

File references
The following examples expand to the name of a ﬁle which is populated with the v alue rendered according to the speciﬁed template. The templateSpec is
deﬁned below.
Map param path
$(params.aMapParam.path)
Array param path
$(params.anArrayParam.path)
Key references
The following examples expand to the v alue of the speciﬁed key (mapP aram) or index (arrayP aram). It does not require a ﬁle to be created. The key must be
a constant v alue.
Map param with subscript
$(params.aMapParam[key])
$(params.aMapParam["key"])
Array param with subscript
$(params.anArrayParam[3])
Star references
In sev eral contexts, an aggregate parameter can be expanded into another aggregate. This does not require a ﬁle to be created.
Expand map or array value into arguments or array parameter bindings
When a star reference appears in an array, the speciﬁed map or array generates a sequence of items deﬁned by a template ( argTemplate  or
arrayTemplate ). For details of the template format, see Aggregate P arameter Array T emplate Speciﬁcation (arrayT emplateSpec) .
The exact template that is used depends on the context.
In args  or command , argTemplate  is used if present, otherwise arrayTemplate  is used:
step:
- args:
- some arg
- $(params.aMapParam[*]) # expanded using argTemplate / arrayTemplate
- some other arg
command:
- some arg
- $(params.aMapParam[*]) # expanded using argTemplate / arrayTemplate
- some other arg
In array parameter bindings, arrayTemplate  is used:
step:
- task: mytask
params:
myArray:
- item1
- $(params.aMapParam[*]) # expanded using arrayTemplate
- item2
Note that argTemplate  primarily exists for backw ard compatibility, and it is recommended that new code use arrayTemplate .
If no template is provided, then:
Arrays are simply copied item by item
Maps are copied by inserting <value>  for each (<key>, <value>)  pair. Note that in this case, the order in which items are generated is unspeciﬁed.
Expand map value in map param binding
In this case, the speciﬁed map is inserted into the outer map.
params:
aMapParam:
a : hello

<!-- Page 28 -->

"..." : $(params.parentMapParam[*])
c : goodbye
If the insert location key (”…” in the abov e example) consists entirely of “.” characters, then each map key is inserted into the outer map as-is. Otherwise the
insert location key is used as a preﬁx to each key in the expanded map. Note that if the insert key ends with multiple “.” characters then all but one of them
are remov ed. This set of rules w as designed to make it easy to insert a map inside another map using both “ﬂat” and “nested” syntax (see Object sugar ).
For example:
params:
aMapParam:
a : hello
"veg." : $(params.parentMapParam[*])
c : goodbye
This adds the preﬁx “v eg.” to each inserted key.
Filtering with a prefix
In all the abov e cases, an optional preﬁx can be provided to limit which keys will be processed. The preﬁx is stripped from the key.
params:
aMapParam:
a : hello
"..." : $(params.parentMapParam[fruit.*])
c : goodbye
This example only matches keys starting with “fruit.”.
Aggregate Parameter Item Render Language
In each of the templates described below, v alues are generated by looping through the key/v alue pairs of the map or array (arrays are treated as maps with
keys 0, 1, … n), substituting the following special v ariables.
Variable Description Example V alue Notes
$(k) key apple
$(K) key, json-escaped and double-quoted “apple”
$(v) value green
$(V) value, json-escaped and double-
quoted“green”
$(n) index of this v alue (0-based) 0 provided for both maps and arrays,
but most useful for arrays
$(N) index of this v alue (1-based) 1 provided for both maps and arrays,
but most useful for arrays
$(L) number of elements 2
$(?) ?, unless this is the last item , ? stands for any other character. This
is useful for separators such as
commas and colons.
The example v alues in the abov e table assume the following map:
map:
apple: green
banana: yellow
Aggregate Parameter T emplate Specification (templateSpec)
Templates consist of strings and loops. Strings can include the escape characters such as “\n” for new line.
Loops are constructed as follows.

<!-- Page 29 -->

$[ ... $]
Both text and special v ariables can be used within a loop.
For example:
params:
- name: aMapParam
render:
template: "{\n$[   $(K): $(V)$(,)\n$]}"
It will generate JSON output as follows.
{
"apple" : "green",
"banana" : "yellow"
}
Note that in this particular case, you can simply use the predeﬁned json template. See the next section for details.
Predefined T emplates
For conv enience, sev eral named templates are provided. They can be used by setting templateName  instead of template .
params:
- name: aMapParam
render:
templateName: json
The following table shows the allow ed v alues of templateName .
Template Array or Map? Description
json Both JSON
map map <key>:<value>  one per line (no escaping)
export map export <key>="<value>"  one per line (v alue
is quoted and JSON-escaped)
conf map <key>=<value>  one per line (no escaping)
list array <item>  one per line (no escaping)
export array export <item>=1  one per line (no escaping)
Aggregate Parameter Array T emplate Specification (arrayT emplateSpec)
Generating arrays works similarly.
params:
- name: aMapParam
render:
arrayTemplate:
- "--start"
- "$["
- "--option"
- "$(k)=$(v)"
- "$]"
- "--end"
This example generates the following items:
- --start
- --option
- apple=green
- --option
- banana=yellow
- --end

<!-- Page 30 -->

Aggregate Parameter Env Specification (aggrEnvSpec)
Environment v ariables can be created from a map or array parameter by setting an envT emplate:
Property Required Default Description Constraints
envT emplate.keyPreﬁx N Only include keys with this
preﬁx (and strip that preﬁx)
envT emplate.namePreﬁx N Preﬁx to prepend to
generated environment
variables
envT emplate.name N Name of environment
variableCan use template language,
e.g. $(k)
envT emplate.v alue N Value of environment
variableCan use template language,
e.g. $(v)
Names and v alues are generated using the abov e template syntax. For example, assume the following map.
apple: green
banana: yellow
params:
- name: aMapParam
envTemplate:
namePrefix: "MYPREFIX_"
This generates the following environment v ariables:
MYPREFIX_apple=green
MYPREFIX_banana=yellow
This is customized using name  and value  together with the template language.
params:
- name: aMapParam
envTemplate:
namePrefix: "ENV_"
name: "$(k)_$(v)"
value: "$(n)"
This generates:
ENV_apple_green=0
ENV_banana_yellow=1
Note that namePrefix  is optional since a preﬁx could just be included in name .
Resource Specification
resources:
- name: name
description: description
default: defaultWRRN
required: true
ui:
name: name
order: 1
componentCategory: scanner
componentType: coverity
resourceCategory: scannerProject
resourceType: coverityProject
role: editor

<!-- Page 31 -->

Property Required Default Description Constraints
name Y Resource name Valid T ekton Name
description N Resource description
default N Default v alue for resource Value is the WRRN of a
resource
required Y Resource description
ui.name N name Display Name for param in
## Ui
ui.order N random Display order of param in UI integer
componentCategory N Component Category
componentType N Component Type Exactly one of
componentCategory and
componentType should be
speciﬁed
resourceCategory N Resource Category
resourceType N Resource Type Exactly one of
componentCategory and
componentType should be
speciﬁed
role Y Pipeline service user must
have this role in selected
resource
Result specification
results:
- name: name
description: description
type: string
Property Required Default Description Constraints
name Y Result name Valid T ekton Name
description Y Result description
type N string Result Type “string” or “array”
Conditional Steps (stepWhenExpr)
You can specify that a step should only be run when one or more conditions are satisﬁed.
The syntax for step conditionals is the same as that for pipeline task conditions (see documentation on Conditional T asks), but with the following limitation:
it must be possible to ev aluate the condition statically when the pipeline is launched. This means that the input  string and values  array must not directly
or indirectly depend on the result of a previous task or previous step (which is considered to be unknown at launch time). Standard v ariable substitutions
may be used in step when  expressions, so long as the abov e constraint is met.
Speciﬁcally, input  and values  can include the following kinds of v ariable substitution:
$(params.paramName)  - refers to a parameter of the task containing this step
$(studio.name) , $(config.name)  - see Variable Substitution

<!-- Page 32 -->

Variable Substitution
Many ﬁelds in tasks support v ariable substitutions:
Variable Description
$(params.<paramName>) value of named parameter
$(params.<renderParamName>.path) ﬁle containing rendered secret, map, or array parameter
$(params.<aggrParamName>[key]) value of key in aggregate (map or array) parameter
$(params.<aggrParamName>.key]) alternativ e syntax for previous case (but if key is “path” then the [] syntax
must be used)
$(resources.<resourceName>[key]) value of property in named resource
$(resources.<resourceName>.key) alternativ e syntax for previous case
$(resources.<resourceName>.secrets.<propertyName>) resource secret; can be used as the v alue of a secret parameter
$(resources.<resourceName>.secrets.<propertyName>.path) ﬁle containing rendered resource secret
$(secrets.<secretName>) named pipeline secret; can be used as the v alue of a secret parameter or
regular parameter or conﬁguration v ariable that will ev entually be used as
the v alue of a secret parameter
$(secrets.<secretName>.path) ﬁle containing named pipeline secret
$(secrets.<secretName>.<key>.path) ﬁle containing speciﬁc key from named pipeline secret
$(results.<resultName>.path) path in which to store result resultName
$(steps.<stepNameOrNumber>.results.<resultName>.path) path holding the result of earlier step in the same task
$(steps.<stepNameOrNumber>.exitCode.path) path holding the exit code of earlier step in the same task
$(tasks.<taskName>.results.<resultName> value of result resultName  of earlier task taskName
$(studio.<propertyName>) value of Studio conﬁguration property
$(config.<propertyName>) value of Pipeline conﬁguration property
$(storage.<storageName>.path) Pipeline T ask storage mount path (can only be used to set parameters at the
Pipeline T ask lev el)
The following studio conﬁguration properties are av ailable by default:
Variable Description
$(studio.home) Studio base domain
$(studio.homeHttp) Studio base url
$(studio.artifacts) Studio artifacts repository domain
$(studio.artifactsHttp) Studio artifacts repository url

<!-- Page 33 -->

Variable Description
$(studio.artifactBuckets) Studio artifacts repository bucket base
$(studio.artifactBucketsHttp) Studio artifacts repository bucket url - to form a URL referencing an artifact,
use
$(studio.artifactBucketsHttp)/${ARTIFACT_BUCKET}/browse/${ARTIF
$(studio.deviceRegistry) Device Registry domain
$(studio.deviceRegistryHttp) Device Registry url
$(studio.dfl) Studio Digital Feedback Loop domain
$(studio.dflHttp) Studio Digital Feedback Loop url
$(studio.gitlab) Studio gitlab domain
$(studio.gitlabHttp) Studio gitlab url
$(studio.jenkins) Studio jenkins domain
$(studio.jenkinsHttp) Studio jenkins url
$(studio.plm) Studio Pipeline Manager domain
$(studio.plmHttp) Studio Pipeline Manager url
$(studio.systemRegistryStudio) System Registry host with registry project name
$(studio.systemRegistry) System Registry host
$(studio.systemRegistryHttp) System Registry url
$(studio.taf) Studio T est Automation Framework domain
$(studio.tafHttp) Studio T est Automation Framework url
$(studio.vault) Studio V ault domain
$(studio.vaultHttp) Studio V ault url
$(studio.vlab) Virtual Lab domain
$(studio.vlabHttp) Virtual Lab url
$(studio.cliImage) Container image including studio-cli and other commonly used utilities
The following pipeline conﬁguration properties are av ailable by default. Note that the user can also deﬁne their own conﬁguration properties, and ev en
override these default settings if needed:
Variable Description
$(config.pipeline.id) Unique Id of current pipeline
$(config.pipeline.name) Machine friendly name (slug) of current pipeline

<!-- Page 34 -->

Variable Description
$(config.pipeline.uiName) Human friendly name of current pipeline
$(config.pipeline.runNumber) Integer identifying the current run
$(config.pipeline.taskName) Machine friendly name of the current pipeline task (block)
$(config.pipeline.workspace) Workspace directory. See environment v ariable ${WORKSPACE}
$(config.pipeline.taskRetries) Value of the pipeline task retries  ﬁeld. Usually 0. Note that this can only
be used in a pipeline (e.g. to set the v alue of a pipeline task param), not in a
task deﬁnition
$(config.pipeline.taskRetryCount) Number of times this pipeline task has been retried (can only be non-zero if
the pipeline task retries  ﬁeld is non-zero)
$(config.pipeline.runBy) Username who started the run (since 24.11 release)
Finally some conv enience environment v ariables are provided. Note that these are only be av ailable if not ov erriden by user settings.
Environment V ariable Description
WORKSP ACE Workspace directory av ailable to each container. Containers within a
pipeline task share a workspace, while containers in diﬀerent pipeline tasks
are allocated diﬀerent (pod local) workspaces. W ill use the v alue of
$(config.pipeline.workspace)
In addition users can add additional conﬁguration properties using the “conﬁg” block in a pipeline deﬁnition. This can be used to deﬁne “global v ariables”
that can be used by task library tasks using the $(config.name)  syntax. Note that “name” can include dots. This feature has all the pros and cons of
global v ariables, so should be used sparingly.
Resource bindings, resource defaults, secret parameter bindings, and secret parameter defaults, can reference pipeline and studio conﬁguration using
$(config.name)  and $(studio.name) . No other v ariable substitutions are allow ed in these contexts. The reason is that the other substitutions are done
while the pipeline is running, but these v alues hav e to be known when the pipeline is “transpiled” to T ekton.
Examples
Pipeline
name: my-pipeline
description: My fancy pipeline
ui:
name: My Pipeline
params:
- name: url
- name: artifactPath
default: workspace-vxworks/projects/tekton/3
#                                -- test1-alpha -->
#                               |                  |
#             -- build-alpha -->                    --
#            |                  |                  |  |
#            |                   -- test2-alpha -->   |
# prepare -->                                          --> analyze
#            |                                        |
#            |                                        |
#             ------- build-beta --> test-beta -------
tasks:
- name: prepare
taskSpec:
steps:
- image: bash:latest

<!-- Page 35 -->

script: |
echo Preparing to run ...
- parallelTasks:
- tasks:
- name: build-alpha
task: build
params:
app: alpha
- parallelTasks:
- name: test1-alpha
task: test1
params:
binary: $(tasks.build-alpha.results.binary)
- name: test2-alpha
task: test2
params:
binary: $(tasks.build-alpha.results.binary)
- tasks:
- name: build-beta
task: build
params:
app: beta
- name: test-beta
task: test
params:
binary: $(tasks.build-beta.results.binary)
- name: analyze
taskSpec:
steps:
- task: analyze-build-results
- task: analyze-test-results
Task
Simple
name: build
description: Build stuff
ui:
name: Build
params:
- name: dir
default: /workspace/source
steps:
- image: bash:latest
command:
- 'build'
- '$(params.dir)'
Combination
name: clone-build-push
description: Clone some code, build it, and push artifacts
ui:
name: Clone Build Push
params:
- name: buildDir
- name: baseArtifactsPath
steps:
- task: clone
params:
dir: $(params.buildDir)
- task: build
params:
dir: $(params.buildDir)
- task: push-artifacts
params:
artifactPath: $(params.baseArtifactsPath)/app

<!-- Page 36 -->

Pipeline Example with Final T ask
name: pipeline-final
ui:
name: Pipeline Final
description: ""
tasks:
- name: echo
params:
string: This is a task in the main section
task: Utils/echo@1.0.0
final: # Final section
- name: echo2
params:
sleep: "0"
string: This is a task in the final section
task: Utils/echo@1.0.0
Pipeline Example with Compute
name: pipeline-compute
ui:
name: Pipeline Compute
description: ""
tasks:
- name: powershell
params:
script: |-
echo "hello from windows"
compute: windows-compute # Reference to the compute that was defined in the compute section
task: Utils/powershell@1.0.0
compute: # Compute section
- name: windows-compute # In this example since it is using a windows task it will need a windows compute
resource: windows-compute-resource
Pipeline Example with Persistent Storage
name: pipeline-storage
ui:
name: Pipeline Storage
storage:
- name: storage1 # Storage name
resource: pvc-efs # PLM Storage resource name
readOnly: false # Defaults to false, i.e. read/write
mountPath: /volumes/storage1 # Volume mount path
- name: storage2 # Storage name
resource: pvc-ebs # PLM Storage resource name
readOnly: false # Defaults to false, i.e. read/write
mountPath: /volumes/storage2 # Volume mount path
tasks:
- name: studio-cli-writer
task: Utils/studio-cli@1.0.0
storage:
- storage: storage1 # Pipeline Storage name
params:
script: |-
cd $(storage.storage1.path)
echo "write in storage"
echo "some value" > use-case-test.txt
- name: studio-cli-reader
task: Utils/studio-cli@1.0.0
storage:
- storage: storage1 # Pipeline Storage name
readOnly: true # Changing default value so now it is a read-only storage for this task
- storage: storage2 # Pipeline Storage name
params:
script: |-
echo "read storage1"
cd $(storage.storage1.path)
cat use-case-test.txt
echo "copy file from storage1 into storage 2"
cp  use-case-test.txt $(storage.storage2.path)
echo "read storage2"

<!-- Page 37 -->

cd $(storage.storage2.path)
cat use-case-test.txt
echo "failure writing in storage1"
cd $(storage.storage1.path)
echo "some value" > new-file.txt
Pipeline Example with Container Settings - Simple T ask
name: pipeline-container-setting-simple
ui:
name: Pipeline Container Setting Simple
description: ”"
tasks:
- name: echo
params:
string: test
task: Utils/echo@1.0.0
steps:
- name: print  # This name must match the step name in the Utils/echo@1.0.0  definition
containerSettings: container-example  # This is the Container Settings alias from the containerSettings key.
containerSettings: # Container settings section
- name: container-example
resource: container-1
Pipeline Example with Container Settings - Complex T ask (use of microtask concept)
name: pipeline-container-setting-complex
ui:
name: Pipeline Container Setting Complex
description: ""
tasks:
- name: super
params:
text: test
task: Utils/super-task@1.0.0
steps:
- name: micro # Name of the step part of Utils/super-task@1.0.0
steps: # Since micro is referencing a microtask that has steps, it is necessary to define a new sub level of steps (tree
representation)
- name: execute # Name of the step part of Utils/simple-task@1.0.0
containerSettings: container-example
- name: results # Name of the step part of Utils/simple-task@1.0.0
containerSettings: container-example-2
containerSettings: # Container settings section
- name: container-example
resource: container-1
containerSettings:
- name: container-example-2
resource: container-2
Pipeline Example with Pipeline W orkspace
name: pipeline-workspace
ui:
name: Pipeline Workspace
tasks:
- name: studio-cli # This task is using the workspace
params:
script: |-
echo $(workspace.my-workspace.path)
echo $(workspace.my-workspace.claim)
echo $(workspace.my-workspace.volume)
echo $(workspace.my-workspace.bound)
task: Utils/studio-cli@1.0.0
workspace:
name: my-workspace
- name: studio-cli-2 # This task is not using the workspace
params:
script: |-
echo $(storage.my-storage.path)
task: Utils/studio-cli@1.0.0
storage:
- storage: my-storage

<!-- Page 38 -->

compute:
- name: my-compute
resource: compute-resource
storage:
- name: my-storage
resource: storage-resource-1
mountPath: /my-storage
workspace: # Workspace section
name: my-workspace
template: # This workspace is using a template instead of a persistent storage
size: 10Gi
accessMode:
- ReadWriteOnce
compute: my-compute
Pipeline Example with T imeouts
name: pipeline-timeouts
ui:
name: Pipeline Timeouts
params:
- name: pipelineTimeout
default: "1h"
- name: taskTimeout
default: "100s"
timeout: $(params.pipelineTimeout) # Timeout set for the pipeline
tasks:
- name: first
timeout: $(params.taskTimeout) # Timeout set for the task
task: sleep
params:
timeSeconds: "30"
Pipeline Example with Conditional T asks
name: pipeline-conditional
ui:
name: Pipeline Conditional
params:
- name: buildMode
default: optimized
- name: preCheck
default: "yes"
- name: postCheck
default: "yes"
tasks:
- task: build
params:
status: passed
- task: do
when: # Conditional task section, both conditions have to be true
- input: $(tasks.build.results.status) # execute if the status of the previous Build task is "passed" or "success"
operator: in
values:
- passed
- success
- input: $(params.buildMode) # Execute if the pipeline param buildMode is set to "optimized" or "debug"
operator: in
values:
- optimized
- debug
params:
runPreCheck: $(params.preCheck)
script: |
echo WhenIn
echo Expect this to run
echo
runPostCheck: $(params.postCheck)

<!-- Page 39 -->

Task Example with Conditional Steps
name: task-conditional
category: Misc
version: "1.0.0"
ui:
name: Task Conditional
params:
- name: script
ui:
multiline: true
- name: runPreCheck
default: "yes"
- name: runPostCheck
default: "yes"
steps:
- name: precheck
when: # Conditional step section
- input: $(params.runPreCheck) # Execute if task param runPreCheck is set to "yes"
operator: in
values: ["yes"]
script: |
echo "Running PreCheck"
- script: |
$(params.script)
- name: postcheck
when: # Conditional step section
- input: $(params.runPostCheck) # Execute if task param runPostCheck is set to "yes"
operator: in
values: ["yes"]
script: |
echo "Result of PreCheck was `cat $(steps.precheck.exitCode.path)`"
echo "Running PostCheck"
Task Example using Unstable State
name: task-unstable
category: Test
version: "1.0.0"
ui:
name: Task Unstable
params:
- name: param
results:
- name: output
description: new result
steps:
- name: main
script: |
#!/bin/bash
failed_tests=5
if [ "$failed_tests" -gt 0 ]; then
echo "Failed tests: $failed_tests"
echo "Test failed during the execution, setting it as unstable"
echo "true" > $(results.unstable.path)
fi
