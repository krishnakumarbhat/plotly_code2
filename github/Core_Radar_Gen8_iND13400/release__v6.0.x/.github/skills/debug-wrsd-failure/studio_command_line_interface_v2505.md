# Studio Command Line Interface, v25.05 (Markdown Conversion)

<!-- Page 1 -->

## Stu Dio Command Line  Int Erface,
V25. 05

<!-- Page 2 -->

Copyright Notice
Copyright © 2026 W ind Riv er Systems, Inc.
All rights reserv ed. No part of this publication may be reproduced or transmitted in any form or by any means
without the prior written permission of W ind Riv er Systems, Inc.
Wind Riv er, Tornado, and VxWorks are registered trademarks of W ind Riv er Systems, Inc. Helix, Pulsar, Rocket,
Titanium Cloud, Titanium Control, Titanium Core, Titanium Edge, Titanium Edge SX, Titanium Serv er, and the
Wind Riv er logo are trademarks of W ind Riv er Systems, Inc. Any third-party trademarks referenced are the
property of their respectiv e owners. For further information regarding W ind Riv er trademarks, please see:
www.windriv er.com/company/terms/trademark.html
This product may include softw are licensed to W ind Riv er by third parties. Relev ant notices (if any) are provided
for your product on the W ind Riv er download and installation portal:
https://gallery.windriv er.com/
Wind Riv er may refer to third-party documentation by listing publications or providing links to third-party
websites for informational purposes. W ind Riv er accepts no responsibility for the information provided in such
third-party documentation.
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
Studio Command Line Interface, v25.05
17 February 2026

<!-- Page 3 -->

Contents
Studio Command Line Interface, v25.051
Studio CLI Contents2
Studio CLI Install and Overview
studio-cli
studio-cli artifacts
studio-cli artifacts bucket
studio-cli artifacts bucket create
studio-cli artifacts bucket delete
studio-cli artifacts bucket get
studio-cli artifacts bucket search
studio-cli artifacts bucket update
studio-cli artifacts folder
studio-cli artifacts folder create
studio-cli artifacts folder delete
studio-cli artifacts folder get
studio-cli artifacts folder search
studio-cli artifacts folder update
studio-cli artifacts group
studio-cli artifacts group assign
studio-cli artifacts group revoke
studio-cli artifacts mc
studio-cli artifacts object
studio-cli artifacts object create
studio-cli artifacts object delete
studio-cli artifacts object get
studio-cli artifacts object search
studio-cli artifacts object update
studio-cli artifacts token
studio-cli artifacts token get
studio-cli completion
studio-cli completion bash
studio-cli completion fish
studio-cli completion powershell
studio-cli completion zsh

<!-- Page 4 -->

studio-cli config
studio-cli config debug
studio-cli config debug set
studio-cli config login
studio-cli config logout
studio-cli config profile
studio-cli config profile add
studio-cli config profile delete
studio-cli config profile get
studio-cli config profile list
studio-cli config profile set
studio-cli config token
studio-cli config token decode
studio-cli config token get
studio-cli config token set
studio-cli config token show
studio-cli config update
studio-cli config userdata
studio-cli devreg
studio-cli devreg group
studio-cli devreg group assign
studio-cli devreg group revoke
studio-cli devreg http
studio-cli devreg project
studio-cli devreg project create
studio-cli devreg project delete
studio-cli devreg project get
studio-cli devreg project group
studio-cli devreg project group add
studio-cli devreg project group delete
studio-cli devreg project group update
studio-cli devreg project list
studio-cli devreg project members
studio-cli devreg project members list
studio-cli devreg project repo
studio-cli devreg project repo artifacts
studio-cli devreg project repo info
studio-cli devreg project repo list
studio-cli devreg project repo remove
studio-cli devreg project repo scan
studio-cli devreg project repo tag
studio-cli devreg project robot
studio-cli devreg project robot create
studio-cli devreg project robot list
studio-cli devreg project robot remove

<!-- Page 5 -->

studio-cli devreg project search
studio-cli devreg project update
studio-cli devreg project user
studio-cli devreg project user add
studio-cli devreg project user delete
studio-cli devreg project user update
studio-cli devreg token
studio-cli devreg token get
studio-cli devreg user
studio-cli devreg user assign-admin
studio-cli devreg user info
studio-cli devreg user list
studio-cli devreg user unassign-admin
studio-cli dfl
studio-cli dfl certificate
studio-cli dfl certificate get
studio-cli dfl certificate update
studio-cli dfl coldpath
studio-cli dfl coldpath get
studio-cli dfl command
studio-cli dfl command get
studio-cli dfl command send
studio-cli dfl commondimensions
studio-cli dfl commondimensions add
studio-cli dfl commondimensions get
studio-cli dfl device
studio-cli dfl device-type
studio-cli dfl device-type get
studio-cli dfl device create
studio-cli dfl device delete
studio-cli dfl device get
studio-cli dfl device list
studio-cli dfl device update
studio-cli dfl endpoint
studio-cli dfl endpoint get
studio-cli dfl file-transfer
studio-cli dfl file-transfer create
studio-cli dfl file-transfer delete
studio-cli dfl file-transfer get
studio-cli dfl file-transfer update
studio-cli dfl hotpath
studio-cli dfl hotpath get
studio-cli dfl log
studio-cli dfl log get
studio-cli dfl schema

<!-- Page 6 -->

studio-cli dfl schema get
studio-cli dfl statistic
studio-cli dfl statistic get
studio-cli dfl threshold
studio-cli dfl threshold create
studio-cli dfl threshold delete
studio-cli dfl threshold get
studio-cli dfl threshold update
studio-cli gendoc
studio-cli gojq
studio-cli jenkins
studio-cli jenkins config
studio-cli jenkins config edit
studio-cli jenkins folder
studio-cli jenkins folder create
studio-cli jenkins folder delete
studio-cli jenkins folder get
studio-cli jenkins folder search
studio-cli jenkins folder update
studio-cli jenkins freestyle-project
studio-cli jenkins freestyle-project build
studio-cli jenkins freestyle-project build start
studio-cli jenkins freestyle-project build status
studio-cli jenkins freestyle-project build tail
studio-cli jenkins freestyle-project create
studio-cli jenkins freestyle-project delete
studio-cli jenkins freestyle-project get
studio-cli jenkins freestyle-project search
studio-cli jenkins freestyle-project update
studio-cli jenkins group
studio-cli jenkins group assign
studio-cli jenkins group revoke
studio-cli jenkins pipeline
studio-cli jenkins pipeline build
studio-cli jenkins pipeline build start
studio-cli jenkins pipeline build status
studio-cli jenkins pipeline build tail
studio-cli jenkins pipeline create
studio-cli jenkins pipeline delete
studio-cli jenkins pipeline get
studio-cli jenkins pipeline search
studio-cli jenkins pipeline update
studio-cli jenkins token
studio-cli jenkins token add
studio-cli jenkins token list

<!-- Page 7 -->

studio-cli jenkins token remove
studio-cli lxbs
studio-cli lxbs build
studio-cli lxbs build cancel
studio-cli lxbs build get
studio-cli lxbs build list-active
studio-cli lxbs build list-completed
studio-cli lxbs build start
studio-cli lxbs configuration
studio-cli lxbs configuration list-branch
studio-cli lxbs configuration setup-access-config
studio-cli lxbs configuration setup-existing-access-config
studio-cli lxbs configuration teardown-access-config
studio-cli lxbs dashboard
studio-cli lxbs group
studio-cli lxbs group assign
studio-cli lxbs group revoke
studio-cli lxbs project
studio-cli lxbs project archive
studio-cli lxbs project assign-access-config
studio-cli lxbs project clone
studio-cli lxbs project create
studio-cli lxbs project create-bulk
studio-cli lxbs project export
studio-cli lxbs project get
studio-cli lxbs project import
studio-cli lxbs project layer
studio-cli lxbs project layer admin
studio-cli lxbs project layer list
studio-cli lxbs project layer update
studio-cli lxbs project list
studio-cli lxbs project local-conf
studio-cli lxbs project local-conf get
studio-cli lxbs project local-conf put
studio-cli lxbs project remove
studio-cli lxbs project repo
studio-cli lxbs project restore
studio-cli lxbs project update
studio-cli lxbs project update-release
studio-cli ota
studio-cli ota certificates
studio-cli ota certificates generate
studio-cli ota devicegroups
studio-cli ota devicegroups create
studio-cli ota devices

<!-- Page 8 -->

studio-cli ota devices create
studio-cli ota devices get
studio-cli ota devices getall
studio-cli ota groups
studio-cli ota groups getall
studio-cli ota packages
studio-cli ota packages create
studio-cli ota packages delete
studio-cli ota packages get
studio-cli ota packages getall
studio-cli ota packages update
studio-cli ota payloads
studio-cli ota payloads create
studio-cli ota payloads create-automated
studio-cli ota payloads createmultipart
studio-cli ota payloads delete
studio-cli ota payloads get
studio-cli ota payloads getall
studio-cli ota payloads getbackground
studio-cli ota payloads multipartmerge
studio-cli ota payloads sign
studio-cli ota payloads split
studio-cli ota payloads uploadchunk
studio-cli ota payloads uploadcomplete
studio-cli ota releases
studio-cli ota releases create
studio-cli ota releases delete
studio-cli ota releases get
studio-cli ota releases getall
studio-cli ota releases gettargets
studio-cli ota releases selecttargets
studio-cli ota releases setstate
studio-cli ota releases update
studio-cli platformhealth
studio-cli platformhealth http
studio-cli plm
studio-cli plm access-config
studio-cli plm access-config create
studio-cli plm access-config delete
studio-cli plm access-config get
studio-cli plm access-config list
studio-cli plm access-config pipeline
studio-cli plm access-config pipeline list
studio-cli plm access-config user
studio-cli plm access-config user assign

<!-- Page 9 -->

studio-cli plm access-config user list
studio-cli plm access-config user revoke
studio-cli plm group
studio-cli plm group assign
studio-cli plm group join
studio-cli plm group leave
studio-cli plm group revoke
studio-cli plm http
studio-cli plm pipeline
studio-cli plm pipeline create
studio-cli plm pipeline delete
studio-cli plm pipeline get
studio-cli plm pipeline get-access-config
studio-cli plm pipeline list
studio-cli plm pipeline lock
studio-cli plm pipeline prettify
studio-cli plm pipeline rename-param
studio-cli plm pipeline rename-task
studio-cli plm pipeline unlock
studio-cli plm pipeline update
studio-cli plm pipeline weave
studio-cli plm resource
studio-cli plm resource assign
studio-cli plm resource list
studio-cli plm resource revoke
studio-cli plm run
studio-cli plm run cancel
studio-cli plm run events
studio-cli plm run get
studio-cli plm run list
studio-cli plm run log
studio-cli plm run start
studio-cli plm secret
studio-cli plm secret create
studio-cli plm secret delete
studio-cli plm secret get
studio-cli plm secret list
studio-cli plm secret update
studio-cli plm task
studio-cli plm task create
studio-cli plm task delete
studio-cli plm task get
studio-cli plm task group
studio-cli plm task group assign
studio-cli plm task group revoke

<!-- Page 10 -->

studio-cli plm task list
studio-cli plm task lock
studio-cli plm task unlock
studio-cli plm task update
studio-cli plm task upsert
studio-cli plm trigger
studio-cli plm trigger create
studio-cli plm trigger delete
studio-cli plm trigger get
studio-cli plm trigger list
studio-cli plm trigger refresh-secret
studio-cli plm trigger update
studio-cli portal
studio-cli portal http
studio-cli portal license
studio-cli portal license add
studio-cli portal license assign
studio-cli portal license create
studio-cli portal license get
studio-cli portal license report
studio-cli portal license revoke
studio-cli portal list
studio-cli portal resource
studio-cli portal resource create
studio-cli portal resource delete
studio-cli portal resource edit
studio-cli portal resource list
studio-cli portal resource list-nodetypes
studio-cli portal resource sync
studio-cli portal storage
studio-cli portal storage create
studio-cli portal storage delete
studio-cli portal storage edit
studio-cli portal storage search
studio-cli ram
studio-cli ram component
studio-cli ram component-template
studio-cli ram component-template assign
studio-cli ram component-template create
studio-cli ram component-template delete
studio-cli ram component-template read
studio-cli ram component-template revoke
studio-cli ram component-template search
studio-cli ram component create
studio-cli ram component delete

<!-- Page 11 -->

studio-cli ram component get-category
studio-cli ram component get-resource
studio-cli ram component get-state
studio-cli ram component get-type
studio-cli ram component read
studio-cli ram component search
studio-cli ram component update
studio-cli ram environment
studio-cli ram environment create
studio-cli ram environment delete
studio-cli ram environment get-state
studio-cli ram environment read
studio-cli ram environment search
studio-cli ram environment update
studio-cli ram location
studio-cli ram location create
studio-cli ram location delete
studio-cli ram location read
studio-cli ram location search
studio-cli ram resource
studio-cli ram resource-template
studio-cli ram resource-template assign
studio-cli ram resource-template create
studio-cli ram resource-template delete
studio-cli ram resource-template get
studio-cli ram resource-template revoke
studio-cli ram resource-template search
studio-cli ram resource check-health
studio-cli ram resource check-role
studio-cli ram resource check-user-access
studio-cli ram resource create
studio-cli ram resource delete
studio-cli ram resource get
studio-cli ram resource get-category
studio-cli ram resource get-state
studio-cli ram resource get-type
studio-cli ram resource get-users
studio-cli ram resource search
studio-cli ram resource update
studio-cli ram tag
studio-cli ram tag assign
studio-cli ram tag create
studio-cli ram tag delete
studio-cli ram tag get-key
studio-cli ram tag get-value

<!-- Page 12 -->

studio-cli ram tag revoke
studio-cli ram tag search
studio-cli schedule
studio-cli schedule apicall
studio-cli schedule apicall create
studio-cli schedule apicall delete
studio-cli schedule apicall execution
studio-cli schedule apicall execution get
studio-cli schedule apicall get
studio-cli schedule apicall list
studio-cli schedule apicall update
studio-cli scm
studio-cli scm access-token
studio-cli scm access-token add
studio-cli scm access-token list
studio-cli scm access-token remove
studio-cli scm branch
studio-cli scm branch create
studio-cli scm branch delete
studio-cli scm branch get
studio-cli scm branch search
studio-cli scm branch update
studio-cli scm gitlab
studio-cli scm glab
studio-cli scm group
studio-cli scm group assign
studio-cli scm group create
studio-cli scm group delete
studio-cli scm group get
studio-cli scm group revoke
studio-cli scm group search
studio-cli scm group update
studio-cli scm project
studio-cli scm project create
studio-cli scm project delete
studio-cli scm project get
studio-cli scm project search
studio-cli scm project update
studio-cli scm ssh-key
studio-cli scm ssh-key add
studio-cli scm ssh-key list
studio-cli scm ssh-key remove
studio-cli secure
studio-cli secure group
studio-cli secure group assign

<!-- Page 13 -->

studio-cli secure group revoke
studio-cli secure http
studio-cli secure secret
studio-cli secure secret create
studio-cli secure secret delete
studio-cli secure secret get
studio-cli secure secret search
studio-cli secure secret update
studio-cli secure token
studio-cli secure token get
studio-cli secure vault
studio-cli sysreg
studio-cli sysreg group
studio-cli sysreg group assign
studio-cli sysreg group revoke
studio-cli sysreg http
studio-cli sysreg project
studio-cli sysreg project create
studio-cli sysreg project delete
studio-cli sysreg project get
studio-cli sysreg project group
studio-cli sysreg project group add
studio-cli sysreg project group delete
studio-cli sysreg project group update
studio-cli sysreg project list
studio-cli sysreg project members
studio-cli sysreg project members list
studio-cli sysreg project repo
studio-cli sysreg project repo artifacts
studio-cli sysreg project repo info
studio-cli sysreg project repo list
studio-cli sysreg project repo remove
studio-cli sysreg project repo scan
studio-cli sysreg project repo tag
studio-cli sysreg project robot
studio-cli sysreg project robot create
studio-cli sysreg project robot list
studio-cli sysreg project robot remove
studio-cli sysreg project search
studio-cli sysreg project update
studio-cli sysreg project user
studio-cli sysreg project user add
studio-cli sysreg project user delete
studio-cli sysreg project user update
studio-cli sysreg token

<!-- Page 14 -->

studio-cli sysreg token get
studio-cli sysreg user
studio-cli sysreg user assign-admin
studio-cli sysreg user info
studio-cli sysreg user list
studio-cli sysreg user unassign-admin
studio-cli taf
studio-cli taf agent
studio-cli taf agent add
studio-cli taf agent delete
studio-cli taf agent get
studio-cli taf agent list
studio-cli taf execution
studio-cli taf execution cancel
studio-cli taf execution get
studio-cli taf execution get-ssh-details
studio-cli taf execution list
studio-cli taf execution release-target
studio-cli taf execution rerun
studio-cli taf execution result
studio-cli taf execution run
studio-cli taf generate
studio-cli taf generate project
studio-cli taf group
studio-cli taf group assign
studio-cli taf group revoke
studio-cli taf health-check
studio-cli taf logs
studio-cli taf logs automation-test-logs
studio-cli taf plan
studio-cli taf plan archive
studio-cli taf plan clone
studio-cli taf plan create
studio-cli taf plan get
studio-cli taf plan list
studio-cli taf plan rename
studio-cli taf plan update
studio-cli taf plugin
studio-cli taf plugin arguments
studio-cli taf plugin install
studio-cli taf plugin list
studio-cli taf plugin status
studio-cli taf plugin uninstall
studio-cli taf project
studio-cli taf project create

<!-- Page 15 -->

studio-cli taf project delete
studio-cli taf project get
studio-cli taf project list
studio-cli taf project test-code-collection
studio-cli taf project test-code-collection append
studio-cli taf project test-code-collection framework
studio-cli taf project test-code-collection list
studio-cli taf project test-code-collection remove
studio-cli taf project test-code-collection test-case
studio-cli taf project test-code-collection test-case get
studio-cli taf project test-code-collection upload
studio-cli taf project update
studio-cli taf suite
studio-cli taf suite archive
studio-cli taf suite create
studio-cli taf suite get
studio-cli taf suite list
studio-cli taf suite update
studio-cli taf target
studio-cli taf target add
studio-cli taf target delete
studio-cli taf target get
studio-cli taf target list
studio-cli um
studio-cli um adminhttp
studio-cli um adminsettings
studio-cli um entitlement
studio-cli um entitlement favorite
studio-cli um entitlement favorite add
studio-cli um entitlement favorite remove
studio-cli um entitlement get
studio-cli um entitlement list
studio-cli um group
studio-cli um group assign
studio-cli um group attribute
studio-cli um group create
studio-cli um group delete
studio-cli um group get
studio-cli um group list
studio-cli um group read
studio-cli um group resource
studio-cli um group resource assign
studio-cli um group resource list
studio-cli um group resource revoke
studio-cli um group revoke

<!-- Page 16 -->

studio-cli um group search
studio-cli um group update
studio-cli um group user
studio-cli um group user assign
studio-cli um group user list
studio-cli um group user revoke
studio-cli um permission
studio-cli um permission get
studio-cli um role
studio-cli um role attribute
studio-cli um role create
studio-cli um role delete
studio-cli um role get
studio-cli um role get-me
studio-cli um role list
studio-cli um role user
studio-cli um role user list
studio-cli um setting
studio-cli um setting get
studio-cli um setting list
studio-cli um setting update
studio-cli um user
studio-cli um user audit
studio-cli um user create
studio-cli um user delete
studio-cli um user disable
studio-cli um user enable
studio-cli um user get
studio-cli um user get-groups
studio-cli um user get-resource
studio-cli um user group
studio-cli um user group assign
studio-cli um user group revoke
studio-cli um user list
studio-cli um user profile
studio-cli um user profile get
studio-cli um user profile picture
studio-cli um user profile picture get
studio-cli um user profile picture update
studio-cli um user profile update
studio-cli um user read
studio-cli um user reset-password
studio-cli um user role
studio-cli um user role assign
studio-cli um user role revoke

<!-- Page 17 -->

studio-cli um user search
studio-cli vlab
studio-cli vlab physical
studio-cli vlab physical create
studio-cli vlab physical delete
studio-cli vlab physical info
studio-cli vlab physical poweroff
studio-cli vlab physical poweron
studio-cli vlab physical reboot
studio-cli vlab physical reserve
studio-cli vlab physical search
studio-cli vlab physical slc
studio-cli vlab physical slc connect
studio-cli vlab physical slc file
studio-cli vlab physical slc file cancel
studio-cli vlab physical slc file configuration
studio-cli vlab physical slc file download
studio-cli vlab physical slc file get
studio-cli vlab physical slc file list
studio-cli vlab physical slc file target-script-download
studio-cli vlab physical slc file target-script-get
studio-cli vlab physical slc file upload
studio-cli vlab physical slc gateway
studio-cli vlab physical slc gateway associate
studio-cli vlab physical slc gateway disconnect
studio-cli vlab physical slc gateway extend-time
studio-cli vlab physical slc gateway list
studio-cli vlab physical slc gateway provision
studio-cli vlab physical slc gateway register
studio-cli vlab physical slc gateway unregister
studio-cli vlab physical slc power-off
studio-cli vlab physical slc power-on
studio-cli vlab physical slc reboot
studio-cli vlab physical slc target
studio-cli vlab physical slc target act
studio-cli vlab physical slc target get-version
studio-cli vlab physical slc target health-check
studio-cli vlab physical slc target kill
studio-cli vlab physical slc target list
studio-cli vlab physical slc target log
studio-cli vlab physical status-change
studio-cli vlab physical unreserve
studio-cli vlab property
studio-cli vlab property create
studio-cli vlab property delete

<!-- Page 18 -->

studio-cli vlab property list
studio-cli vlab reservation
studio-cli vlab reservation list
studio-cli vlab reservation modify
studio-cli vlab virtual
studio-cli vlab virtual create
studio-cli vlab virtual delete
studio-cli vlab virtual edit
studio-cli vlab virtual export
studio-cli vlab virtual extend
studio-cli vlab virtual reserve
studio-cli vlab virtual search
studio-cli vlab virtual smoke-test
studio-cli vlab virtual ssh
studio-cli vlab virtual ssh key
studio-cli vlab virtual ssh key add
studio-cli vlab virtual ssh key delete
studio-cli vlab virtual ssh key search
studio-cli vlab virtual ssh user
studio-cli vlab virtual ssh user create
studio-cli vlab virtual ssh user delete
studio-cli vlab virtual ssh user search
studio-cli vlab virtual status
studio-cli vlab virtual unreserve
studio-cli vlab virtual update
studio-cli vxbs
studio-cli vxbs artifact
studio-cli vxbs artifact get
studio-cli vxbs artifact list
studio-cli vxbs artifact list-all
studio-cli vxbs build
studio-cli vxbs build cancel
studio-cli vxbs build get
studio-cli vxbs build list-active
studio-cli vxbs build list-completed
studio-cli vxbs build postbuild
studio-cli vxbs build postbuild add
studio-cli vxbs build postbuild disable
studio-cli vxbs build postbuild enable
studio-cli vxbs build postbuild remove
studio-cli vxbs build prebuild
studio-cli vxbs build prebuild add
studio-cli vxbs build prebuild disable
studio-cli vxbs build prebuild enable
studio-cli vxbs build prebuild remove

<!-- Page 19 -->

studio-cli vxbs build start
studio-cli vxbs build yamlfile
studio-cli vxbs build yamlfile add
studio-cli vxbs build yamlfile disable
studio-cli vxbs build yamlfile enable
studio-cli vxbs build yamlfile remove
studio-cli vxbs configuration
studio-cli vxbs configuration list-board
studio-cli vxbs configuration list-cpu
studio-cli vxbs configuration list-release
studio-cli vxbs configuration setup-access-config
studio-cli vxbs configuration setup-existing-access-config
studio-cli vxbs configuration teardown-access-config
studio-cli vxbs dashboard
studio-cli vxbs group
studio-cli vxbs group assign
studio-cli vxbs group revoke
studio-cli vxbs project
studio-cli vxbs project archive
studio-cli vxbs project assign-access-config
studio-cli vxbs project clone
studio-cli vxbs project close
studio-cli vxbs project get
studio-cli vxbs project list
studio-cli vxbs project list-archived
studio-cli vxbs project remove
studio-cli vxbs project restore
studio-cli vxbs project save
studio-cli vxbs vip
studio-cli vxbs vip bundle
studio-cli vxbs vip bundle add
studio-cli vxbs vip bundle remove
studio-cli vxbs vip component
studio-cli vxbs vip component add
studio-cli vxbs vip component remove
studio-cli vxbs vip create
studio-cli vxbs vip file
studio-cli vxbs vip file get
studio-cli vxbs vip file list
studio-cli vxbs vip file post
studio-cli vxbs vip file put
studio-cli vxbs vip lkm
studio-cli vxbs vip lkm add
studio-cli vxbs vip lkm remove
studio-cli vxbs vip parameter

<!-- Page 20 -->

studio-cli vxbs vip parameter get
studio-cli vxbs vip parameter set
studio-cli vxbs vip romfs
studio-cli vxbs vip romfs file
studio-cli vxbs vip romfs file add
studio-cli vxbs vip romfs folder
studio-cli vxbs vip romfs folder add
studio-cli vxbs vip romfs remove
studio-cli vxbs vsb
studio-cli vxbs vsb add
studio-cli vxbs vsb create
studio-cli vxbs vsb disable
studio-cli vxbs vsb enable
studio-cli vxbs vsb set
studio-cli ws
studio-cli ws instance
studio-cli ws instance create
studio-cli ws instance delete
studio-cli ws instance extend
studio-cli ws instance list
studio-cli ws instance restart
studio-cli ws instance ssh
studio-cli ws instance start
studio-cli ws instance status
studio-cli ws instance stop
studio-cli ws namespace
studio-cli ws namespace provision
studio-cli ws template
studio-cli ws template assign
studio-cli ws template copy
studio-cli ws template create
studio-cli ws template delete
studio-cli ws template list
studio-cli ws template update

<!-- Page 21 -->

1
Studio Command Line Interface, v25.05
Wind Riv er Studio is a cloud based DevSecOps system used to build and deploy your VxWorks and
Wind Riv er Linux dev elopment projects. The Studio Command Line Interface (CLI) enables you to
automate all aspects of Studio from the desktop. All command information in this reference is also
available with the tool’s help command.
Please see Wind Riv er Studio  documentation for additional information.
Studio CLI Contents
The following commands are supported in this release:

<!-- Page 22 -->

Studio CLI Install and Ov erview
studio-cli
studio-cli artifacts
studio-cli artifacts bucket
studio-cli artifacts bucket create
studio-cli artifacts bucket delete
studio-cli artifacts bucket get
studio-cli artifacts bucket search
studio-cli artifacts bucket update
studio-cli artifacts folder
studio-cli artifacts folder create
studio-cli artifacts folder delete
studio-cli artifacts folder get
studio-cli artifacts folder search
studio-cli artifacts folder update
studio-cli artifacts group
studio-cli artifacts group assign
studio-cli artifacts group revoke
studio-cli artifacts mc
studio-cli artifacts object
studio-cli artifacts object create
studio-cli artifacts object delete
studio-cli artifacts object get
studio-cli artifacts object search
studio-cli artifacts object update
studio-cli artifacts token
studio-cli artifacts token get
studio-cli completion
studio-cli completion bash
studio-cli completion ﬁsh
studio-cli completion pow ershell
studio-cli completion zsh
studio-cli conﬁg
studio-cli conﬁg debug
studio-cli conﬁg debug set
studio-cli conﬁg login
studio-cli conﬁg logout
studio-cli conﬁg proﬁle
studio-cli conﬁg proﬁle add
studio-cli conﬁg proﬁle delete
studio-cli conﬁg proﬁle get
studio-cli conﬁg proﬁle list
studio-cli conﬁg proﬁle set
studio-cli conﬁg token
studio-cli conﬁg token decode
studio-cli conﬁg token get
studio-cli conﬁg token set
studio-cli conﬁg token show
studio-cli conﬁg update
studio-cli conﬁg userdata

<!-- Page 23 -->

studio-cli devreg
studio-cli devreg group
studio-cli devreg group assign
studio-cli devreg group revoke
studio-cli devreg http
studio-cli devreg project
studio-cli devreg project create
studio-cli devreg project delete
studio-cli devreg project get
studio-cli devreg project group
studio-cli devreg project group add
studio-cli devreg project group delete
studio-cli devreg project group update
studio-cli devreg project list
studio-cli devreg project members
studio-cli devreg project members list
studio-cli devreg project repo
studio-cli devreg project repo artifacts
studio-cli devreg project repo info
studio-cli devreg project repo list
studio-cli devreg project repo remov e
studio-cli devreg project repo scan
studio-cli devreg project repo tag
studio-cli devreg project robot
studio-cli devreg project robot create
studio-cli devreg project robot list
studio-cli devreg project robot remov e
studio-cli devreg project search
studio-cli devreg project update
studio-cli devreg project user
studio-cli devreg project user add
studio-cli devreg project user delete
studio-cli devreg project user update
studio-cli devreg token
studio-cli devreg token get
studio-cli devreg user
studio-cli devreg user assign-admin
studio-cli devreg user info
studio-cli devreg user list
studio-cli devreg user unassign-admin
studio-cli dﬂ
studio-cli dﬂ certiﬁcate
studio-cli dﬂ certiﬁcate get
studio-cli dﬂ certiﬁcate update
studio-cli dﬂ coldpath
studio-cli dﬂ coldpath get
studio-cli dﬂ command
studio-cli dﬂ command get
studio-cli dﬂ command send
studio-cli dﬂ commondimensions

<!-- Page 24 -->

studio-cli dﬂ commondimensions add
studio-cli dﬂ commondimensions get
studio-cli dﬂ device
studio-cli dﬂ device-type
studio-cli dﬂ device-type get
studio-cli dﬂ device create
studio-cli dﬂ device delete
studio-cli dﬂ device get
studio-cli dﬂ device list
studio-cli dﬂ device update
studio-cli dﬂ endpoint
studio-cli dﬂ endpoint get
studio-cli dﬂ ﬁle-transfer
studio-cli dﬂ ﬁle-transfer create
studio-cli dﬂ ﬁle-transfer delete
studio-cli dﬂ ﬁle-transfer get
studio-cli dﬂ ﬁle-transfer update
studio-cli dﬂ hotpath
studio-cli dﬂ hotpath get
studio-cli dﬂ log
studio-cli dﬂ log get
studio-cli dﬂ schema
studio-cli dﬂ schema get
studio-cli dﬂ statistic
studio-cli dﬂ statistic get
studio-cli dﬂ threshold
studio-cli dﬂ threshold create
studio-cli dﬂ threshold delete
studio-cli dﬂ threshold get
studio-cli dﬂ threshold update
studio-cli gendoc
studio-cli gojq
studio-cli jenkins
studio-cli jenkins conﬁg
studio-cli jenkins conﬁg edit
studio-cli jenkins folder
studio-cli jenkins folder create
studio-cli jenkins folder delete
studio-cli jenkins folder get
studio-cli jenkins folder search
studio-cli jenkins folder update
studio-cli jenkins freesty le-project
studio-cli jenkins freesty le-project build
studio-cli jenkins freesty le-project build start
studio-cli jenkins freesty le-project build status
studio-cli jenkins freesty le-project build tail
studio-cli jenkins freesty le-project create
studio-cli jenkins freesty le-project delete
studio-cli jenkins freesty le-project get
studio-cli jenkins freesty le-project search

<!-- Page 25 -->

studio-cli jenkins freesty le-project update
studio-cli jenkins group
studio-cli jenkins group assign
studio-cli jenkins group revoke
studio-cli jenkins pipeline
studio-cli jenkins pipeline build
studio-cli jenkins pipeline build start
studio-cli jenkins pipeline build status
studio-cli jenkins pipeline build tail
studio-cli jenkins pipeline create
studio-cli jenkins pipeline delete
studio-cli jenkins pipeline get
studio-cli jenkins pipeline search
studio-cli jenkins pipeline update
studio-cli jenkins token
studio-cli jenkins token add
studio-cli jenkins token list
studio-cli jenkins token remov e
studio-cli lxbs
studio-cli lxbs build
studio-cli lxbs build cancel
studio-cli lxbs build get
studio-cli lxbs build list-activ e
studio-cli lxbs build list-completed
studio-cli lxbs build start
studio-cli lxbs conﬁguration
studio-cli lxbs conﬁguration list-branch
studio-cli lxbs conﬁguration setup-access-conﬁg
studio-cli lxbs conﬁguration setup-existing-access-conﬁg
studio-cli lxbs conﬁguration teardown-access-conﬁg
studio-cli lxbs dashboard
studio-cli lxbs group
studio-cli lxbs group assign
studio-cli lxbs group revoke
studio-cli lxbs project
studio-cli lxbs project archiv e
studio-cli lxbs project assign-access-conﬁg
studio-cli lxbs project clone
studio-cli lxbs project create
studio-cli lxbs project create-bulk
studio-cli lxbs project export
studio-cli lxbs project get
studio-cli lxbs project import
studio-cli lxbs project lay er
studio-cli lxbs project lay er admin
studio-cli lxbs project lay er list
studio-cli lxbs project lay er update
studio-cli lxbs project list
studio-cli lxbs project local-conf
studio-cli lxbs project local-conf get

<!-- Page 26 -->

studio-cli lxbs project local-conf put
studio-cli lxbs project remov e
studio-cli lxbs project repo
studio-cli lxbs project restore
studio-cli lxbs project update
studio-cli lxbs project update-release
studio-cli ota
studio-cli ota certiﬁcates
studio-cli ota certiﬁcates generate
studio-cli ota devicegroups
studio-cli ota devicegroups create
studio-cli ota devices
studio-cli ota devices create
studio-cli ota devices get
studio-cli ota devices getall
studio-cli ota groups
studio-cli ota groups getall
studio-cli ota packages
studio-cli ota packages create
studio-cli ota packages delete
studio-cli ota packages get
studio-cli ota packages getall
studio-cli ota packages update
studio-cli ota pay loads
studio-cli ota pay loads create
studio-cli ota pay loads create-automated
studio-cli ota pay loads createmultipart
studio-cli ota pay loads delete
studio-cli ota pay loads get
studio-cli ota pay loads getall
studio-cli ota pay loads getbackground
studio-cli ota pay loads multipartmerge
studio-cli ota pay loads sign
studio-cli ota pay loads split
studio-cli ota pay loads uploadchunk
studio-cli ota pay loads uploadcomplete
studio-cli ota releases
studio-cli ota releases create
studio-cli ota releases delete
studio-cli ota releases get
studio-cli ota releases getall
studio-cli ota releases gettargets
studio-cli ota releases selecttargets
studio-cli ota releases setstate
studio-cli ota releases update
studio-cli platformhealth
studio-cli platformhealth http
studio-cli plm
studio-cli plm access-conﬁg
studio-cli plm access-conﬁg create

<!-- Page 27 -->

studio-cli plm access-conﬁg delete
studio-cli plm access-conﬁg get
studio-cli plm access-conﬁg list
studio-cli plm access-conﬁg pipeline
studio-cli plm access-conﬁg pipeline list
studio-cli plm access-conﬁg user
studio-cli plm access-conﬁg user assign
studio-cli plm access-conﬁg user list
studio-cli plm access-conﬁg user revoke
studio-cli plm group
studio-cli plm group assign
studio-cli plm group join
studio-cli plm group leav e
studio-cli plm group revoke
studio-cli plm http
studio-cli plm pipeline
studio-cli plm pipeline create
studio-cli plm pipeline delete
studio-cli plm pipeline get
studio-cli plm pipeline get-access-conﬁg
studio-cli plm pipeline list
studio-cli plm pipeline lock
studio-cli plm pipeline prettify
studio-cli plm pipeline rename-param
studio-cli plm pipeline rename-task
studio-cli plm pipeline unlock
studio-cli plm pipeline update
studio-cli plm pipeline w eave
studio-cli plm resource
studio-cli plm resource assign
studio-cli plm resource list
studio-cli plm resource revoke
studio-cli plm run
studio-cli plm run cancel
studio-cli plm run ev ents
studio-cli plm run get
studio-cli plm run list
studio-cli plm run log
studio-cli plm run start
studio-cli plm secret
studio-cli plm secret create
studio-cli plm secret delete
studio-cli plm secret get
studio-cli plm secret list
studio-cli plm secret update
studio-cli plm task
studio-cli plm task create
studio-cli plm task delete
studio-cli plm task get
studio-cli plm task group

<!-- Page 28 -->

studio-cli plm task group assign
studio-cli plm task group revoke
studio-cli plm task list
studio-cli plm task lock
studio-cli plm task unlock
studio-cli plm task update
studio-cli plm task upsert
studio-cli plm trigger
studio-cli plm trigger create
studio-cli plm trigger delete
studio-cli plm trigger get
studio-cli plm trigger list
studio-cli plm trigger refresh-secret
studio-cli plm trigger update
studio-cli portal
studio-cli portal http
studio-cli portal license
studio-cli portal license add
studio-cli portal license assign
studio-cli portal license create
studio-cli portal license get
studio-cli portal license report
studio-cli portal license revoke
studio-cli portal list
studio-cli portal resource
studio-cli portal resource create
studio-cli portal resource delete
studio-cli portal resource edit
studio-cli portal resource list
studio-cli portal resource list-nodetypes
studio-cli portal resource sync
studio-cli portal storage
studio-cli portal storage create
studio-cli portal storage delete
studio-cli portal storage edit
studio-cli portal storage search
studio-cli ram
studio-cli ram component
studio-cli ram component-template
studio-cli ram component-template assign
studio-cli ram component-template create
studio-cli ram component-template delete
studio-cli ram component-template read
studio-cli ram component-template revoke
studio-cli ram component-template search
studio-cli ram component create
studio-cli ram component delete
studio-cli ram component get-category
studio-cli ram component get-resource
studio-cli ram component get-state

<!-- Page 29 -->

studio-cli ram component get-type
studio-cli ram component read
studio-cli ram component search
studio-cli ram component update
studio-cli ram environment
studio-cli ram environment create
studio-cli ram environment delete
studio-cli ram environment get-state
studio-cli ram environment read
studio-cli ram environment search
studio-cli ram environment update
studio-cli ram location
studio-cli ram location create
studio-cli ram location delete
studio-cli ram location read
studio-cli ram location search
studio-cli ram resource
studio-cli ram resource-template
studio-cli ram resource-template assign
studio-cli ram resource-template create
studio-cli ram resource-template delete
studio-cli ram resource-template get
studio-cli ram resource-template revoke
studio-cli ram resource-template search
studio-cli ram resource check-health
studio-cli ram resource check-role
studio-cli ram resource check-user-access
studio-cli ram resource create
studio-cli ram resource delete
studio-cli ram resource get
studio-cli ram resource get-category
studio-cli ram resource get-state
studio-cli ram resource get-type
studio-cli ram resource get-users
studio-cli ram resource search
studio-cli ram resource update
studio-cli ram tag
studio-cli ram tag assign
studio-cli ram tag create
studio-cli ram tag delete
studio-cli ram tag get-key
studio-cli ram tag get-v alue
studio-cli ram tag revoke
studio-cli ram tag search
studio-cli schedule
studio-cli schedule apicall
studio-cli schedule apicall create
studio-cli schedule apicall delete
studio-cli schedule apicall execution
studio-cli schedule apicall execution get

<!-- Page 30 -->

studio-cli schedule apicall get
studio-cli schedule apicall list
studio-cli schedule apicall update
studio-cli scm
studio-cli scm access-token
studio-cli scm access-token add
studio-cli scm access-token list
studio-cli scm access-token remov e
studio-cli scm branch
studio-cli scm branch create
studio-cli scm branch delete
studio-cli scm branch get
studio-cli scm branch search
studio-cli scm branch update
studio-cli scm gitlab
studio-cli scm glab
studio-cli scm group
studio-cli scm group assign
studio-cli scm group create
studio-cli scm group delete
studio-cli scm group get
studio-cli scm group revoke
studio-cli scm group search
studio-cli scm group update
studio-cli scm project
studio-cli scm project create
studio-cli scm project delete
studio-cli scm project get
studio-cli scm project search
studio-cli scm project update
studio-cli scm ssh-key
studio-cli scm ssh-key add
studio-cli scm ssh-key list
studio-cli scm ssh-key remov e
studio-cli secure
studio-cli secure group
studio-cli secure group assign
studio-cli secure group revoke
studio-cli secure http
studio-cli secure secret
studio-cli secure secret create
studio-cli secure secret delete
studio-cli secure secret get
studio-cli secure secret search
studio-cli secure secret update
studio-cli secure token
studio-cli secure token get
studio-cli secure v ault
studio-cli sysreg
studio-cli sysreg group

<!-- Page 31 -->

studio-cli sysreg group assign
studio-cli sysreg group revoke
studio-cli sysreg http
studio-cli sysreg project
studio-cli sysreg project create
studio-cli sysreg project delete
studio-cli sysreg project get
studio-cli sysreg project group
studio-cli sysreg project group add
studio-cli sysreg project group delete
studio-cli sysreg project group update
studio-cli sysreg project list
studio-cli sysreg project members
studio-cli sysreg project members list
studio-cli sysreg project repo
studio-cli sysreg project repo artifacts
studio-cli sysreg project repo info
studio-cli sysreg project repo list
studio-cli sysreg project repo remov e
studio-cli sysreg project repo scan
studio-cli sysreg project repo tag
studio-cli sysreg project robot
studio-cli sysreg project robot create
studio-cli sysreg project robot list
studio-cli sysreg project robot remov e
studio-cli sysreg project search
studio-cli sysreg project update
studio-cli sysreg project user
studio-cli sysreg project user add
studio-cli sysreg project user delete
studio-cli sysreg project user update
studio-cli sysreg token
studio-cli sysreg token get
studio-cli sysreg user
studio-cli sysreg user assign-admin
studio-cli sysreg user info
studio-cli sysreg user list
studio-cli sysreg user unassign-admin
studio-cli taf
studio-cli taf agent
studio-cli taf agent add
studio-cli taf agent delete
studio-cli taf agent get
studio-cli taf agent list
studio-cli taf execution
studio-cli taf execution cancel
studio-cli taf execution get
studio-cli taf execution get-ssh-details
studio-cli taf execution list
studio-cli taf execution release-target

<!-- Page 32 -->

studio-cli taf execution rerun
studio-cli taf execution result
studio-cli taf execution run
studio-cli taf generate
studio-cli taf generate project
studio-cli taf group
studio-cli taf group assign
studio-cli taf group revoke
studio-cli taf health-check
studio-cli taf logs
studio-cli taf logs automation-test-logs
studio-cli taf plan
studio-cli taf plan archiv e
studio-cli taf plan clone
studio-cli taf plan create
studio-cli taf plan get
studio-cli taf plan list
studio-cli taf plan rename
studio-cli taf plan update
studio-cli taf plugin
studio-cli taf plugin arguments
studio-cli taf plugin install
studio-cli taf plugin list
studio-cli taf plugin status
studio-cli taf plugin uninstall
studio-cli taf project
studio-cli taf project create
studio-cli taf project delete
studio-cli taf project get
studio-cli taf project list
studio-cli taf project test-code-collection
studio-cli taf project test-code-collection append
studio-cli taf project test-code-collection framework
studio-cli taf project test-code-collection list
studio-cli taf project test-code-collection remov e
studio-cli taf project test-code-collection test-case
studio-cli taf project test-code-collection test-case get
studio-cli taf project test-code-collection upload
studio-cli taf project update
studio-cli taf suite
studio-cli taf suite archiv e
studio-cli taf suite create
studio-cli taf suite get
studio-cli taf suite list
studio-cli taf suite update
studio-cli taf target
studio-cli taf target add
studio-cli taf target delete
studio-cli taf target get
studio-cli taf target list

<!-- Page 33 -->

studio-cli um
studio-cli um adminhttp
studio-cli um adminsettings
studio-cli um entitlement
studio-cli um entitlement favorite
studio-cli um entitlement favorite add
studio-cli um entitlement favorite remov e
studio-cli um entitlement get
studio-cli um entitlement list
studio-cli um group
studio-cli um group assign
studio-cli um group attribute
studio-cli um group create
studio-cli um group delete
studio-cli um group get
studio-cli um group list
studio-cli um group read
studio-cli um group resource
studio-cli um group resource assign
studio-cli um group resource list
studio-cli um group resource revoke
studio-cli um group revoke
studio-cli um group search
studio-cli um group update
studio-cli um group user
studio-cli um group user assign
studio-cli um group user list
studio-cli um group user revoke
studio-cli um permission
studio-cli um permission get
studio-cli um role
studio-cli um role attribute
studio-cli um role create
studio-cli um role delete
studio-cli um role get
studio-cli um role get-me
studio-cli um role list
studio-cli um role user
studio-cli um role user list
studio-cli um setting
studio-cli um setting get
studio-cli um setting list
studio-cli um setting update
studio-cli um user
studio-cli um user audit
studio-cli um user create
studio-cli um user delete
studio-cli um user disable
studio-cli um user enable
studio-cli um user get

<!-- Page 34 -->

studio-cli um user get-groups
studio-cli um user get-resource
studio-cli um user group
studio-cli um user group assign
studio-cli um user group revoke
studio-cli um user list
studio-cli um user proﬁle
studio-cli um user proﬁle get
studio-cli um user proﬁle picture
studio-cli um user proﬁle picture get
studio-cli um user proﬁle picture update
studio-cli um user proﬁle update
studio-cli um user read
studio-cli um user reset-password
studio-cli um user role
studio-cli um user role assign
studio-cli um user role revoke
studio-cli um user search
studio-cli v lab
studio-cli v lab physical
studio-cli v lab physical create
studio-cli v lab physical delete
studio-cli v lab physical info
studio-cli v lab physical pow eroﬀ
studio-cli v lab physical pow eron
studio-cli v lab physical reboot
studio-cli v lab physical reserv e
studio-cli v lab physical search
studio-cli v lab physical slc
studio-cli v lab physical slc connect
studio-cli v lab physical slc ﬁle
studio-cli v lab physical slc ﬁle cancel
studio-cli v lab physical slc ﬁle conﬁguration
studio-cli v lab physical slc ﬁle download
studio-cli v lab physical slc ﬁle get
studio-cli v lab physical slc ﬁle list
studio-cli v lab physical slc ﬁle target-script-download
studio-cli v lab physical slc ﬁle target-script-get
studio-cli v lab physical slc ﬁle upload
studio-cli v lab physical slc gatew ay
studio-cli v lab physical slc gatew ay associate
studio-cli v lab physical slc gatew ay disconnect
studio-cli v lab physical slc gatew ay extend-time
studio-cli v lab physical slc gatew ay list
studio-cli v lab physical slc gatew ay provision
studio-cli v lab physical slc gatew ay register
studio-cli v lab physical slc gatew ay unregister
studio-cli v lab physical slc pow er-oﬀ
studio-cli v lab physical slc pow er-on
studio-cli v lab physical slc reboot

<!-- Page 35 -->

studio-cli v lab physical slc target
studio-cli v lab physical slc target act
studio-cli v lab physical slc target get-v ersion
studio-cli v lab physical slc target health-check
studio-cli v lab physical slc target kill
studio-cli v lab physical slc target list
studio-cli v lab physical slc target log
studio-cli v lab physical status-change
studio-cli v lab physical unreserv e
studio-cli v lab property
studio-cli v lab property create
studio-cli v lab property delete
studio-cli v lab property list
studio-cli v lab reserv ation
studio-cli v lab reserv ation list
studio-cli v lab reserv ation modify
studio-cli v lab virtual
studio-cli v lab virtual create
studio-cli v lab virtual delete
studio-cli v lab virtual edit
studio-cli v lab virtual export
studio-cli v lab virtual extend
studio-cli v lab virtual reserv e
studio-cli v lab virtual search
studio-cli v lab virtual smoke-test
studio-cli v lab virtual ssh
studio-cli v lab virtual ssh key
studio-cli v lab virtual ssh key add
studio-cli v lab virtual ssh key delete
studio-cli v lab virtual ssh key search
studio-cli v lab virtual ssh user
studio-cli v lab virtual ssh user create
studio-cli v lab virtual ssh user delete
studio-cli v lab virtual ssh user search
studio-cli v lab virtual status
studio-cli v lab virtual unreserv e
studio-cli v lab virtual update
studio-cli vxbs
studio-cli vxbs artifact
studio-cli vxbs artifact get
studio-cli vxbs artifact list
studio-cli vxbs artifact list-all
studio-cli vxbs build
studio-cli vxbs build cancel
studio-cli vxbs build get
studio-cli vxbs build list-activ e
studio-cli vxbs build list-completed
studio-cli vxbs build postbuild
studio-cli vxbs build postbuild add
studio-cli vxbs build postbuild disable

<!-- Page 36 -->

studio-cli vxbs build postbuild enable
studio-cli vxbs build postbuild remov e
studio-cli vxbs build prebuild
studio-cli vxbs build prebuild add
studio-cli vxbs build prebuild disable
studio-cli vxbs build prebuild enable
studio-cli vxbs build prebuild remov e
studio-cli vxbs build start
studio-cli vxbs build y amlﬁle
studio-cli vxbs build y amlﬁle add
studio-cli vxbs build y amlﬁle disable
studio-cli vxbs build y amlﬁle enable
studio-cli vxbs build y amlﬁle remov e
studio-cli vxbs conﬁguration
studio-cli vxbs conﬁguration list-board
studio-cli vxbs conﬁguration list-cpu
studio-cli vxbs conﬁguration list-release
studio-cli vxbs conﬁguration setup-access-conﬁg
studio-cli vxbs conﬁguration setup-existing-access-conﬁg
studio-cli vxbs conﬁguration teardown-access-conﬁg
studio-cli vxbs dashboard
studio-cli vxbs group
studio-cli vxbs group assign
studio-cli vxbs group revoke
studio-cli vxbs project
studio-cli vxbs project archiv e
studio-cli vxbs project assign-access-conﬁg
studio-cli vxbs project clone
studio-cli vxbs project close
studio-cli vxbs project get
studio-cli vxbs project list
studio-cli vxbs project list-archiv ed
studio-cli vxbs project remov e
studio-cli vxbs project restore
studio-cli vxbs project sav e
studio-cli vxbs vip
studio-cli vxbs vip bundle
studio-cli vxbs vip bundle add
studio-cli vxbs vip bundle remov e
studio-cli vxbs vip component
studio-cli vxbs vip component add
studio-cli vxbs vip component remov e
studio-cli vxbs vip create
studio-cli vxbs vip ﬁle
studio-cli vxbs vip ﬁle get
studio-cli vxbs vip ﬁle list
studio-cli vxbs vip ﬁle post
studio-cli vxbs vip ﬁle put
studio-cli vxbs vip lkm
studio-cli vxbs vip lkm add

<!-- Page 37 -->

studio-cli vxbs vip lkm remov e
studio-cli vxbs vip parameter
studio-cli vxbs vip parameter get
studio-cli vxbs vip parameter set
studio-cli vxbs vip romfs
studio-cli vxbs vip romfs ﬁle
studio-cli vxbs vip romfs ﬁle add
studio-cli vxbs vip romfs folder
studio-cli vxbs vip romfs folder add
studio-cli vxbs vip romfs remov e
studio-cli vxbs vsb
studio-cli vxbs vsb add
studio-cli vxbs vsb create
studio-cli vxbs vsb disable
studio-cli vxbs vsb enable
studio-cli vxbs vsb set
studio-cli ws
studio-cli ws instance
studio-cli ws instance create
studio-cli ws instance delete
studio-cli ws instance extend
studio-cli ws instance list
studio-cli ws instance restart
studio-cli ws instance ssh
studio-cli ws instance start
studio-cli ws instance status
studio-cli ws instance stop
studio-cli ws namespace
studio-cli ws namespace provision
studio-cli ws template
studio-cli ws template assign
studio-cli ws template copy
studio-cli ws template create
studio-cli ws template delete
studio-cli ws template list
studio-cli ws template update

<!-- Page 38 -->

2
Studio CLI Contents
Studio CLI Install and Overview
Command line tool for interfacing with Studio REST APIs.
Installing studio-cli
You can cut and paste the following commands into your shell window for the following platforms.
Installing in Bash Shell for Linux 64bit, MacOS 64bit
You can cut and paste the single line below:
INST_URL=https://distro.windriver.com/dist/wrstudio/wrstudio-cli-distro-cd/install-studio-
cli.sh && curl -f $INST_URL --output inst.sh && bash inst.sh -u $INST_URL
Or you run the commands individually:
INST_URL=https://distro.windriver.com/dist/wrstudio/wrstudio-cli-distro-cd/install-studio-
cli.sh
curl -f $INST_URL --output inst.sh
bash inst.sh -u $INST_URL
Installing for W indows 10 64bit cmd.exe shell
To install studio-cli.exe and its associated binaries:
winget install studio-cli
If you encounter ‘Failed when searching source: msstore’, you can try:
winget install studio-cli --source winget
After installation, you need open a new Command Prompt window, so P ATH v ariable containing
studio-cli will take eﬀect.

<!-- Page 39 -->

To upgrade studio-cli.exe and its associated binaries:
winget upgrade studio-cli
Command line flag syntax
Command line ﬂag syntax is compatible with the GNU extensions to the POSIX recommendations for
command-line options .
boolean flags
Use –ﬂag when boolean ﬂags Examples: -b –bool
only on flags without a default value
Use –ﬂag x when non-boolean and ﬂags without a ‘no option default v alue’. Examples: -n 1234 –str
“abcd”
mixed
The format –ﬂag=x can be used either boolean ﬂag or on ﬂags without a default v alue Examples: -
b=true -n=1234 –str=”hello” but -b true is INV ALID
Flag parsing stops after the terminator “–“.
boolean flags
Boolean ﬂags (in their long form) accept 1, 0, t, f, true, false, TRUE, F ALSE, True, False.
integer flags
Integer ﬂags accept 1234, 0664, 0x1234 and may be negativ e.
string flags
String ﬂags accept string wrapper with “ or ‘, especially useful when there is space in ﬂag option.
strings (Slice of Strings) flags
Slice of strings ﬂags, accept a list of strings, which can be v alues separated using “,”, or provided
multiple times. Example: –strs “abc, def, gh” –strs abc –strs def –strs gh
stringArray (Array of Strings) flags
Array of Strings ﬂags accept an array of strings with speciﬁed name and v alue, which can be provided
multiple times. Example: –arrs a=abc –arrs b=def
Environment V ariables
These are the environment v ariables which can ov erride what ev er is set in the conﬁguration ﬁle. On
Linux, the conﬁguration ﬁle is $HOME/.studio-cli/studiocfg.y aml ﬁle. And one W indows, the
conﬁguration ﬁle is C:\Users<Username>.studio-cli\studiocfg.y aml ﬁle.
When reporting a problem alw ays set MY_DEBUGHTTP=3 and provide all the logs.

<!-- Page 40 -->

Variable Description
MY_PW password for studio user
MY_STUDIO base name of studio instance
MY_USER studio user name
STUDIO_OUTPUT Output type set to json or y aml (acts like –output)
MY_DEBUGHTTP If set to 1, 2, or 3, act like –debughttp, –
debughttp2, or –debughttp3 w as entered on the
command line.
MY_PROFILE Override the –proﬁle option
studio-cli core commands
Command description
studio-cli artifacts Artifacts repository command group
studio-cli completion Generate the autocompletion script for the
speciﬁed shell
studio-cli conﬁg Manage user conﬁguration data
studio-cli devreg Device registry conﬁguration commands
studio-cli dﬂ Digital Feedback Loop
studio-cli gendoc Generate command reference document
studio-cli gojq Act as gojq which can read/write/modify json and
yaml
studio-cli help Help about any command
studio-cli jenkins Conﬁg jenkins
studio-cli lxbs Linux Build System commands

<!-- Page 41 -->

Command description
studio-cli ota OTA commands
studio-cli platformhealth Platform Health commands
studio-cli plm Pipeline Manager
studio-cli portal Main Control Program
studio-cli ram Ram commands (previous rbac)
studio-cli schedule Schedule (Studio 23.03 and later )
studio-cli scm Source Code Management Commands
studio-cli secure Security related commands
studio-cli sysreg System registry conﬁguration commands
studio-cli taf Test Automation Framework Commands
studio-cli um User management commands
studio-cli v lab Physical/V irtual target control commands
studio-cli vxbs VxWorks Build System commands
studio-cli ws Workspace Management commands
Artifacts
In order to access most artifacts such as copy to and from the serv er, you will need to hav e the mc
optional binary installed. Y ou also need to hav e the proper role assigned to your account to be able to
write to a artifact storage location, if you are trying to copy an object to the serv er. When the optional
mc binary is downloaded and installed in the same directory as studio-cli you will be able to use the
“studio-cli artifacts mc” wrapper command, which obtains a temporal minio token as needed before
executing mc.
You also hav e the option of generating a temporary token which can be used with an external tools like
mc using the mctoken  command.

<!-- Page 42 -->

Artifacts commands
Command description
studio-cli artifacts bucket Artifacts bucket command group
studio-cli artifacts folder Artifacts folder command group
studio-cli artifacts group Group commands
studio-cli artifacts mc Mc wrapper command, requires mc installed with
studio-cli
studio-cli artifacts object Artifacts object command group
studio-cli artifacts token Artifacts token command group
Config
By default the studio-cli will attempt to sav e your password using ssh-key encryption. If you don’t
want studio-cli to encrypt and remember your password you can run the following command shown
below a single time. Or you could add export STUDIO_SA VEPW=no-sav e to your shell startup.
STUDIO_SAVEPW=no-save studio-cli config userdata
For nearly all the operations the studio-cli will obtain an authorization token which is cached for a
period of time determined by the settings in the WR Studio instance.
Config Commands
Command description
studio-cli conﬁg debug Conﬁg debug command group
studio-cli conﬁg login Initiate or complete a remote login when using
Azure AAD
studio-cli conﬁg logout Logout from wrstudio
studio-cli conﬁg proﬁle Conﬁg proﬁle command group
studio-cli conﬁg token Conﬁg token command group

<!-- Page 43 -->

Command description
studio-cli conﬁg update Update to latest binary by re-running the install
studio-cli conﬁg userdata Conﬁgure studio cli defaults
To enable command line completion with bash run the following command:
complete -C studio-cli studio-cli
SCM (Source Code Management) commands
The scm command can be used to list the current set of keys you hav e installed into your scm proﬁle
and add additional keys. An ssh key is required to push to gitlab as w ell as for ssh access. Y ou can
generate and add a new key as follows:
ssh-keygen -o -a 100 -t ed25519 -f ~/.ssh/id_ed25519
studio-cli scm sshadd -k ~/.ssh/id_ed25519.pub
Command description
studio-cli scm access-token Scm access-token command group
studio-cli scm branch Scm gitlab branch command group.
studio-cli scm gitlab Wrapper for python-gitlab command (requires
python-gitlab to be installed)
studio-cli scm glab Wrapper for glab command, requires glab is
installed with studio-cli
studio-cli scm group Scm gitlab group command group.
studio-cli scm project Scm gitlab project command group.
studio-cli scm ssh-key Scm ssh-key command group
Harbor Device / System Registry commands
The “studio-cli devreg” accesses the WR Studio device registry and the “studio-cli sysreg” accesses the
WR Studio system registry. In the tables below only the devreg commands will be shown. All of the
commands are the same for the system registry by sw apping “devreg” and “sysreg”. Only a subset of

<!-- Page 44 -->

the harbor commands are implemented in studio-cli. Y ou can use the studio-cli http wrapper to access
the full
It is possible to use the entire Harbor API by using the “studio-cli devreg http” wrapper, which will
take care of all the OpenID and CSRF token authentication. Y ou can view all the Harbor API
commands by logging into one of the device registries and then visiting the link to “Harbor API V2.0”
in the low er left hand corner.
Here is a Harbor API example
% studio-cli devreg http -t GET -e /statistics
{
"private_project_count": 0,
"private_repo_count": 0,
"public_project_count": 11,
"public_repo_count": 29
}
The “studio-cli devreg secret” command allows you to get the access token for your account which you
can use to perform a docker login or use a limited set of curl commands with the Harbor API.
devreg commands
Command Description
studio-cli devreg group Group commands
studio-cli devreg http Submit calls directly to the REST API
studio-cli devreg project Project manipulation commands
studio-cli devreg token Token commands
studio-cli devreg user User commands
devreg project commands
Command Description
studio-cli devreg project create Create a container registry project
studio-cli devreg project delete Delete resource
studio-cli devreg project get Get resource
studio-cli devreg project group Group commands

<!-- Page 45 -->

Command Description
studio-cli devreg project list List container registry projects
studio-cli devreg project members Members commands
studio-cli devreg project repo Repo commands
studio-cli devreg project robot Robot commands
studio-cli devreg project search Search resource
studio-cli devreg project update Update a container registry project
studio-cli devreg project user User commands
Here is an example of creating a project and a robot:
% studio-cli devreg project create -n test-proj
{
"success": true
}
% studio-cli devreg project robotcreate -n test-proj -r pushbot
{
"name": "robot$pushbot",
"token":
"eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE2MzM4Nzg0MzMsImlhdCI6MTYzMTI4NjQzMywiaXNzI
joiaGFyYm9yLXRva2VuLWRlZmF1bHRJc3N1ZXIiLCJpZCI6MzYsInBpZCI6NTEsImFjY2VzcyI6W3siUmVzb3VyY2Ui
OiIvcHJvamVjdC81MS9oZWxtLWNoYXJ0LXZlcnNpb24iLCJBY3Rpb24iOiJjcmVhdGUiLCJFZmZlY3QiOiIifSx7IlJ
lc291cmNlIjoiL3Byb2plY3QvNTEvaGVsbS1jaGFydC12ZXJzaW9uIiwiQWN0aW9uIjoiY3JlYXRlIiwiRWZmZWN0Ij
oiIn0seyJSZXNvdXJjZSI6Ii9wcm9qZWN0LzUxL2hlbG0tY2hhcnQtdmVyc2lvbiIsIkFjdGlvbiI6ImNyZWF0ZSIsI
kVmZmVjdCI6IiJ9XX0.SpEIJK0WGI0shUX0OTyFt77Mm41Ia5zSkpbjoU9ISqKRpS8oTX61HK6PWQVn0DacETSqP5dT
rHHaizg9W0-C7HBgevGAi_w-
KjP1uKlxXlUBbKzShM9qpHiseKmXzUNrVlRo5ST8qu6a6l3wy0xLJgeybSNgQmUvty6uiRrJGx6X4ttrHSwS64NYIbX
w29WqRcnmOej7Rj9NnaDED43LEBYJP37EMLxv-J4VQkQYnUXEtP1kxq5VZvtuKNaDMLl95-xPhD-
RBLxQ6ulYQg295uXBs0-x20F70SoSSTKIMMml_rqxQFWvwrZSSQPLV0J-
DRQGV3iDKo4F3FZ57SXlEaaHoibIEVm2bosNvDoXa6NQXcyVlI1WszcOq5H7dnwlsiwYe34kCuuxtbQsznlKU7GA2Xk
CBsgot8btfHmCYDxEu76ymjLylmGlMOYss03-
h_ePGaPs5QLMA7FGYDCUTddQlMFl39P50gSEtftGvP4f_lZ6O1mNDl0gtlr6w45Yb4ufp2EOemkitWmyiHnYvUyIjj9
ddsT0ZhXvd_r4VlWlrdfKQFKbLWNaz5ny1jEvltBKrMih28x0Pfwtnd__LBuunaXH5BO2qIXsTfnxdgrzVUU6BhiEsB
439OG8rbk2oQmI7aUU5Jv5Df-1_PEKNg5bA-b1564Pmj6_Lav2I3E"
}
NOTE: The priv ate token is only printed a single time and cannot be retriev ed again
Here is another example to show you how you could capture the token for further scripting and test a
container push.

<!-- Page 46 -->

export MY_STUDIO=training-aws.wrstudio
studio-cli devreg project create -n test-proj
token=`studio-cli devreg project robotcreate -n test-proj -r pushbot --jq '.token' --raw`
docker login -u 'robot$pushbot' -p $token device.registry.${MY_STUDIO}/test-proj
docker pull busybox:latest
docker tag busybox:latest device.registry.${MY_STUDIO}/test-proj/busybox:latest
docker push device.registry.${MY_STUDIO}/test-proj/busybox:latest
Virtual Lab (vlab)
The virtual lab system can manage both physical and virtual targets.
Virtual Lab sub-commands
Command Description
studio-cli v lab physical Manage physical targets.
studio-cli v lab property Manage v lab properties.
studio-cli v lab reserv ation Manage reserv ation.
studio-cli v lab virtual Manage simulation targets in virtual lab
Virtual Lab virtual target commands
Command Description
studio-cli v lab virtual create Create a virtual target deﬁnition using a
json/y aml ﬁle (needs admin right for v lab targets
created by others).
studio-cli v lab virtual delete Delete virtual target deﬁnition (needs admin right
for v lab targets created by others).
studio-cli v lab virtual edit Edit a virtual target deﬁnition using a json/y aml
ﬁle (needs admin right for v lab targets created by
others).
studio-cli v lab virtual export Export virtual target deﬁnition to a json template.
studio-cli v lab virtual extend Extend the lease on an existing reserv ation.

<!-- Page 47 -->

Command Description
studio-cli v lab virtual reserv e Reserv e a single target by id.
studio-cli v lab virtual search Search for targets.
studio-cli v lab virtual smoke-test Run a smoke test.
studio-cli v lab virtual ssh Run the ssh wrapper for a reserv ation and or
dynamically add your ssh key
studio-cli v lab virtual status Show the status of the reserv ation
studio-cli v lab virtual unreserv e Unreserv e a target by reserv ation id, by name or
all.
studio-cli v lab virtual
studio-cli v lab virtual Use –group with groupnames (comma separated) to
unreserv e all targets in a group.
studio-cli v lab virtual update Update v ariables target deﬁnition attributes
(needs admin right for v lab targets created by
others)
Virtual Lab physical target commands
Command Description
studio-cli v lab physical create Create a physical target deﬁnition using a json ﬁle
(requires admin).
studio-cli v lab physical delete Delete a physical target deﬁnition by id (requires
admin)
studio-cli v lab physical info Show all details of a particular target
studio-cli v lab physical pow eroﬀ Set pow er oﬀ physical target
studio-cli v lab physical pow eron Set pow er on physical target
studio-cli v lab physical reboot Reboot physical target by id (coming soon)

<!-- Page 48 -->

Command Description
studio-cli v lab physical reserv e Create Physical target reserv ation now or in the
future.
studio-cli v lab physical search Search for physical targets
studio-cli v lab physical slc Manage SLC (Studio Lab Connect) targets.
studio-cli v lab physical status-change Enable or Disable physical targets
studio-cli v lab physical unreserv e Unreserv e Physical target immediately.
Example
You can reserv e a target with the name core-i7 by using the following commands:
# Search for a specific target called core-i7 and reserve it
tgt=`studio-cli vlab virtual search -p QEMU --jq '.data[] | select(.name=="core-i7") | .id'
--raw`
# Reserve the target
studio-cli vlab virtual reserve -i $tgt
# Show the ssh commands
studio-cli vlab virtual showssh
# Now unreserve
studio-cli vlab virtual unreserve -a
The studio-cli v lab virtual createvirty aml command uses a y aml ﬁle to create virtual targets. A sample
yaml is provided in the : cr_virty aml_sample.y aml
MCP  - Main Control Program
Main Control Program Commands
Command Description
studio-cli mcp http Submit calls directly to the REST API
studio-cli mcp license Mcp license command group
studio-cli mcp list List installed WR Studio components and their
versions

<!-- Page 49 -->

Command Description
studio-cli mcp resource Mcp resource command group
Jenkins commands
The jenkins interface will allow you to manage API keys which you can in turn use to call jenkins’s
REST API directly. A tail command w as also provided so that you hav e a clean interface for getting a
job’s console log, or w aiting and w atching the log with polling.
Command Description
studio-cli jenkins conﬁg Conﬁg commands
studio-cli jenkins folder Folder commands
studio-cli jenkins freesty le-project freesty le-project commands
studio-cli jenkins group Group commands
studio-cli jenkins pipeline pipeline commands
studio-cli jenkins token Token commands
Once you create an API token you can use to call jenkins directly. Y ou can ﬁnd examples and
documentation here: https://www.jenkins.io/doc/book/using/remote-access-api/
Pipeline Manager commands
Command Description
studio-cli plm access-conﬁg Pipeline Access Conﬁg commands
studio-cli plm group Pipeline group commands
studio-cli plm http Submit calls directly to the REST API
studio-cli plm pipeline Pipeline commands
studio-cli plm resource Pipeline resource commands

<!-- Page 50 -->

Command Description
studio-cli plm run Pipeline run commands
studio-cli plm secret Pipeline secret commands
studio-cli plm task Pipeline task commands
studio-cli plm trigger Pipeline trigger commands
The “studio-cli plm blockadd” command alw ays requires the name of the pipeline and the block type
you w ant to add. Y ou can optionally specify a name below. Y ou must add the ﬁrst block of a pipeline
using the –newstage, because the pipeline will start oﬀ with zero stages. Each additional block you add
from that point can be added as a new stage or to an existing stage. Y ou may only add blocks of the
same type to a stage presently. Each stage will run in parallel and block until the prior stage has
completed.
# blockadd example for stage 1: vsb   stage 2: vip and sdk (create stages in reverse
order）
studio-cli plm create -n my-reverse-pipeline
studio-cli plm blockadd -n my-reverse-pipeline --btype "VxWorks" -b "VxSDK" --newstage
studio-cli plm blockadd -n my-reverse-pipeline --btype "VxWorks" -b "VxImage" --stage 1
studio-cli plm blockadd -n my-reverse-pipeline --btype "VxWorks" -b "VxSrcBld" --newstage
# blockadd example for stage 1: vsb   stage 2: vip and sdk （ create stages in sequential
order）
studio-cli plm create -n my-sequential-pipeline
studio-cli plm blockadd -n my-sequential-pipeline --btype "VxWorks" -b "VxSrcBld" --
newstage
studio-cli plm blockadd -n my-sequential-pipeline --btype "VxWorks" -b "VxImage" --stage 2
--newstage
studio-cli plm blockadd -n my-sequential-pipeline --btype "VxWorks" -b "VxSDK" --stage 2 --
order 2
Internally the pipeline manager stores all references to projects, targets and blocks as ids. The blockedit
sub command is useful because it will look up projects, targets, and blocks by names or ids. Y ou can
also use the “?” for the v ariables when performing modiﬁcations to a block.
# using blockedit to lookup the parameters for a Linux Build Block
studio-cli plm blockedit -n my-test-pipeline -b VxSDK  -s ?
[
{
"component": "vxbs-project-selector",
"form": true,
"key": "projectId",
"name": "Project ID",
"order": 1,
"type": "vxbs-project-id"
},

<!-- Page 51 -->

{
"component": "select",
"form": true,
"key": "build_options",
"name": "Build Options",
"options": [
{
"name": "VIP BUILD",
"projectType": "1",
"value": "vxworks_vip_build"
},
{
"name": "VSB BUILD",
"projectType": "2",
"value": "vxworks_vsb_build"
},
{
"name": "CONTAINER IMAGE",
"projectType": "1",
"value": "container_image"
},
{
"name": "SDK",
"projectType": "1",
"value": "sdk"
},
{
"name": "ADE",
"projectType": "1",
"value": "ade"
}
],
"order": 2,
"type": "build_options"
}
]
Here is a complete example for a VSB + VIP + SDK -> Open a v lab target assuming you hav e created a
vsp and vip projected named itl_CORE_2107_vsb_1014_01 and itl_CORE_2107_vip_1014_02.
### Add VxWorks Pipeline Blocks
studio-cli plm remove -n IA-VxWorks-Build-and-Test-Pipeline
studio-cli plm create -n IA-VxWorks-Build-and-Test-Pipeline
studio-cli plm blockadd -n IA-VxWorks-Build-and-Test-Pipeline --btype "Virtual Target" -b
"IA_QEMU" --newstage
studio-cli plm blockadd -n IA-VxWorks-Build-and-Test-Pipeline --btype "VxWorks" -b "VxSDK"
--newstage
studio-cli plm blockadd -n IA-VxWorks-Build-and-Test-Pipeline --btype "VxWorks" -b
"VxImage" --stage 1
studio-cli plm blockadd -n IA-VxWorks-Build-and-Test-Pipeline --btype "VxWorks" -b
"VxSrcBld" --newstage
# Configure target blocks
studio-cli plm blockedit -n IA-VxWorks-Build-and-Test-Pipeline -b "IA_QEMU" -s "id=x86-64"
-s artifactPath="VxImage"
# Configure VxWorks Build blocks
studio-cli plm blockedit -n IA-VxWorks-Build-and-Test-Pipeline -b "VxSrcBld" -s

<!-- Page 52 -->

projectId=itl_CORE_2107_vsb_1014_01 -s buildType=vxworks_vsb_build
studio-cli plm blockedit -n IA-VxWorks-Build-and-Test-Pipeline -b "VxImage" -s
projectId=itl_CORE_2107_vip_1014_02 -s buildType=vxworks_vip_build
studio-cli plm blockedit -n IA-VxWorks-Build-and-Test-Pipeline -b "VxSDK" -s
projectId=itl_CORE_2107_vip_1014_02 -s buildType=sdk
Linux build system
Core Linux Build System commands
Command Description
studio-cli lxbs build Build commands
studio-cli lxbs conﬁguration Conﬁguration commands
studio-cli lxbs dashboard Show my projects
studio-cli lxbs group Group commands
studio-cli lxbs project Project commands
The studio-cli lxbs bulkcreate command has uses a y aml ﬁle to create multiple projects. A sample y aml
is provided here: lxbs_project_samples.y aml
Linux “build” sub commands
Command Description
studio-cli lxbs build cancel Cancel a build for a project.
studio-cli lxbs build get Get details of a speciﬁc build.
studio-cli lxbs build list-activ e Search through the activ e builds
studio-cli lxbs build list-completed Search through the completed builds
studio-cli lxbs build start Start the building of the project.

<!-- Page 53 -->

VxW orks Build System commands
Core VxW orks Build System commands
Command Description
studio-cli vxbs artifact Artifact commands.
studio-cli vxbs build Conﬁg build
studio-cli vxbs conﬁguration List av ailable artifacts/board/Cpu/release
studio-cli vxbs dashboard Show my dashboard
studio-cli vxbs group Group commands
studio-cli vxbs project Project commands
studio-cli vxbs vip Conﬁg VIP
studio-cli vxbs vsb Conﬁg VSB
VxW orks “create” sub commands
Command Description
studio-cli vxbs create artifact Artifact commands.
studio-cli vxbs create build Conﬁg build
studio-cli vxbs create conﬁguration List av ailable artifacts/board/Cpu/release
studio-cli vxbs create dashboard Show my dashboard
studio-cli vxbs create group Group commands
studio-cli vxbs create project Project commands
studio-cli vxbs create vip Conﬁg VIP
studio-cli vxbs create vsb Conﬁg VSB

<!-- Page 54 -->

VxW orks “config” sub commands
Command Description
studio-cli vxbs conﬁg artifact Artifact commands.
studio-cli vxbs conﬁg build Conﬁg build
studio-cli vxbs conﬁg conﬁguration List av ailable artifacts/board/Cpu/release
studio-cli vxbs conﬁg dashboard Show my dashboard
studio-cli vxbs conﬁg group Group commands
studio-cli vxbs conﬁg project Project commands
studio-cli vxbs conﬁg vip Conﬁg VIP
studio-cli vxbs conﬁg vsb Conﬁg VSB
VxW orks “build” sub commands
Command Description
studio-cli vxbs build cancel Cancel build
studio-cli vxbs build get Get build details
studio-cli vxbs build list-activ e Get activ e builds
studio-cli vxbs build list-completed Get completed builds
studio-cli vxbs build postbuild Manager post build for projects
studio-cli vxbs build prebuild Manager prebuild for projects
studio-cli vxbs build start Build start
studio-cli vxbs build y amlﬁle Manager y aml ﬁle for projects

<!-- Page 55 -->

VxW orks “list” sub commands
Command Description
studio-cli vxbs list artifact Artifact commands.
studio-cli vxbs list build Conﬁg build
studio-cli vxbs list conﬁguration List av ailable artifacts/board/Cpu/release
studio-cli vxbs list dashboard Show my dashboard
studio-cli vxbs list group Group commands
studio-cli vxbs list project Project commands
studio-cli vxbs list vip Conﬁg VIP
studio-cli vxbs list vsb Conﬁg VSB
Tozny Administration
You must hav e the administration role in order to run the user/group/role lev el controls.
Tozny commands
Command Description
studio-cli tozny adminhttp Submit calls directly to the Tozny Admin Console
## Rest Api
studio-cli tozny adminsettings Manage default realm token settings in the admin
console
studio-cli tozny entitlement Entitlement manager
studio-cli tozny group Group management commands
studio-cli tozny permission Permission manager
studio-cli tozny role User management role commands

<!-- Page 56 -->

Command Description
studio-cli tozny setting Setting manager
studio-cli tozny user Tozny user commands
studio-cli tozny group assign Assign a role to a group in tozny.
studio-cli tozny group attribute Display, add or remov e an attribute from a group.
studio-cli tozny group create Create a new group.
studio-cli tozny group delete Delete a group
studio-cli tozny group get Get data of a group by group name.
studio-cli tozny group list Get groups.
studio-cli tozny group read Read a group in tozny.
studio-cli tozny group resource Resource manager
studio-cli tozny group revoke Revoke a role from a group in tozny.
studio-cli tozny group search Search group
studio-cli tozny group update Update a group.
studio-cli tozny group user Group user management commands
studio-cli tozny role attribute Display, add or remov e an attribute from a role.
studio-cli tozny role create Create a new role.
studio-cli tozny role delete Delete a role.
studio-cli tozny role get Get role info
studio-cli tozny role get-me Get the roles of the current user.
studio-cli tozny role get-user Get a list of users that belong to a role.

<!-- Page 57 -->

Command Description
studio-cli tozny role list Get all roles in the system
studio-cli tozny role read Read a role or all roles from tozny.
studio-cli tozny role user User operations
studio-cli tozny user audit Create an audit entry for user actions.
studio-cli tozny user create Create a new user
studio-cli tozny user delete Delete an user.
studio-cli tozny user disable Set user enabled to false
studio-cli tozny user enable Set user enabled to true
studio-cli tozny user get Get user info with details.
studio-cli tozny user get-groups Get groups from user.
studio-cli tozny user get-resource Get the list of resources a user has access.
studio-cli tozny user group User group commands
studio-cli tozny user list Get a list of users in the system.
studio-cli tozny user proﬁle User personal proﬁle manager
studio-cli tozny user read Read user data.
studio-cli tozny user reset-password Request a password reset for a speciﬁc user
studio-cli tozny user role User role commands
studio-cli tozny user search Search user by ﬁltering and paging.

<!-- Page 58 -->

Vault Commands
You must a v ault reader or writer role for the account you are using in order to successfully perform
any v ault operation.
Command Description
studio-cli secure group Group commands
studio-cli secure http Curl sty le access with dynamic tokens to the v ault
## Api
studio-cli secure secret Secure secret command group
studio-cli secure token Secure token command group
studio-cli secure v ault Vault wrapper command, requires v ault installed
with studio-cli
TAF commands
Studio-cli taf commands are listed here.
Command Description
studio-cli taf agent Taf agent commands
studio-cli taf execution Execution commands
studio-cli taf generate Generate commands
studio-cli taf group Group commands
studio-cli taf logs TAF Logs commands
studio-cli taf plan Test plan commands
studio-cli taf plugin Plugin commands
studio-cli taf project Project commands

<!-- Page 59 -->

Command Description
studio-cli taf suite Test suite commands
studio-cli taf target Taf target commands
studio-cli
Wind Riv er Studio Command Line Interface tool
Synopsis
Studio-cli is a Command Line Interface (CLI) application for W ind Riv er Studio that empow ers user
operation. It supports operation on diﬀerent WR Studio subsystems like vxbs, lxbs, v lab, artifacts, etc…
You can use it in manual command prompt or automatic scripting environments.
Proﬁles:
The studio-cli supports the use of the –proﬁle for setting and remembering the username, password
and MY_STUDIO deployment name.
The –proﬁle and –cfgﬁle arguments must come before any sub commands to the studi-cli.
To create, or edit a proﬁle, you can simply run:
studio-cli --profile YOUR_PROFILE_NAME config userdata
You can also specify default v alues for environment v ariables which are set on each invocation of
studio-cli unless they hav e been ov erridden by an environment v ariable which has been set in by the
shell running studio-cli. Each v ariable you set must be separated by a semicolon. Example: # Set the
proﬁle name to “ex” else it will use the default proﬁle export STUDIO_PROFILE=ex # List the
conﬁguration of the proﬁle studio-cli conﬁg userdata -l # Set EDITOR to emacs studio-cli conﬁg
userdata –env=”MY_DEBUGHTTP=1;EDITOR=/usr/bin/emacs” # ov erride editor to vi export
EDITOR=/usr/bin/vi studio-cli …command that uses the editor… # Clear the env defaults for the
proﬁle studio-cli conﬁg userdata –env=””
Using Multiple Conﬁguration ﬁles with completion and aliases:
You can alternativ ely use bash aliases with a speciﬁc conﬁguration ﬁle for ev ery WR Studio
deployment. Example in your ~/.bashrc ﬁle:
# Alias the wrsd command to a specific config file
alias wrsd="studio-cli --cfgfile=~/.mystudio.yaml"
complete -C "studio-cli studio-cli --cfgfile=~/.mystudio.yaml" wrsd
# Or you could use use a profile with an alias
alias t-cli="studio-cli --profile=testenv"
complete -C "studio-cli studio-cli --profile=testenv" t-cli
Environment v ariables:

<!-- Page 60 -->

Each environment v ariable will take precedence ov er any command line argument.
STUDIO_CFGFILE - Ov erride the the command line and default cfgﬁle STUDIO_PROFILE - Ov erride
the the command line and default proﬁle MY_STUDIO - Ov erride the setting of the MY_STUDIO
location MY_USER - Ov erride the MY_USER v ariable MY_PW - Specify the user password to use use
for authentication MY_DEBUGHTTP - Set to “1”, “2”, or “3” to force v erbose http logging EDITOR -
Invoke this text editor for commands that edit ﬁles
For deployments which hav e external authentication providers the studio-cli will attempt to use an
external provider when ev er the username contains the “@” symbol.
Environment v ariables for external IDP providers like Azure AD: MY_IDP - Specify an IDP provider
for deployments with more than one MY_IDP_PROXY_PORT - Local serv er port for proxying IDP
login. If not conﬁgured, a random av ailable local port will be used. DISABLE_TZ_IDP - Force the use of
a local login with an “@” in the username FORCE_TZ_IDP - Force use an external IDP provider
without “@” in the username
Command line ﬂag syntax:
boolean flags
Use –ﬂag when boolean ﬂags Examples: -b –bool
flags without a default value
Use –ﬂag x when non-boolean and ﬂags without a ‘no option default v alue’. Examples: -n 1234 –str
“abcd”
mixed
The format –ﬂag=x can be used either boolean ﬂag or on ﬂags without a default v alue Examples: -
b=true -n=1234 –str=”hello” but -b true is INV ALID
Flag parsing stops after the terminator “–“.
boolean flags
Boolean ﬂags (in their long form) accept 1, 0, t, f, true, false, TRUE, F ALSE, True, False.
integer flags
Integer ﬂags accept 1234, 0664, 0x1234 and may be negativ e.
string flags
String ﬂags accept string wrapper with “ or ‘, especially useful when there is space in ﬂag option.
strings (Slice of Strings) flags
Slice of strings ﬂags, accept a list of strings, which can be v alues separated using “,”, or provided
multiple times. Example: –strs “abc, def, gh” –strs abc –strs def –strs gh

<!-- Page 61 -->

stringArray (Array of Strings) flags
Array of Strings ﬂags accept an array of strings with speciﬁed name and v alue, which can be provided
multiple times. Example: –arrs a=abc –arrs b=def
studio-cli [flags]
Options
--cfgfile string         config file (default is $HOME/.studio-cli/studiocfg.yaml)
--command-tree strings   Display all commands
[help,all,colons,spaces,deprecated,deprecated-only]
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-h, --help                   help for studio-cli
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
--profile string         Name of profile to use for studio variables
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
-v, --version                Show version information

<!-- Page 62 -->

## See Also
studio-cli artifacts  - Artifacts repository command group
studio-cli completion  - Generate the autocompletion script for the speciﬁed shell
studio-cli conﬁg  - Manage user conﬁguration data
studio-cli devreg  - Device registry conﬁguration commands
studio-cli dﬂ  - Digital Feedback Loop
studio-cli gendoc  - Generate command reference document
studio-cli gojq  - Act as gojq which can read/write/modify json and y aml
studio-cli jenkins  - Conﬁg jenkins
studio-cli lxbs  - Linux Build System commands
studio-cli ota  - OT A commands
studio-cli platformhealth  - Platform Health commands
studio-cli plm  - Pipeline Manager
studio-cli portal  - Main Control Program
studio-cli ram  - Ram commands (previous rbac)
studio-cli schedule  - Schedule (Studio 23.03 and later )
studio-cli scm  - Source Code Management Commands
studio-cli secure  - Security related commands
studio-cli sysreg  - System registry conﬁguration commands
studio-cli taf  - Test Automation Framework Commands
studio-cli um  - User management commands
studio-cli v lab - Physical/V irtual target control commands
studio-cli vxbs  - VxWorks Build System commands
studio-cli ws  - Workspace Management commands
studio-cli artifacts
Artifacts repository command group
Synopsis
Artifacts repository command group.
Options
-h, --help   help for artifacts

<!-- Page 63 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli artifacts bucket  - Artifacts bucket command group
studio-cli artifacts folder  - Artifacts folder command group
studio-cli artifacts group  - Group commands
studio-cli artifacts mc  - Mc wrapper command, requires mc installed with studio-cli
studio-cli artifacts object  - Artifacts object command group
studio-cli artifacts token  - Artifacts token command group
studio-cli artifacts bucket
Artifacts bucket command group
Synopsis
Artifacts bucket command group
Options
-h, --help        help for bucket
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 64 -->

## See Also
studio-cli artifacts  - Artifacts repository command group
studio-cli artifacts bucket create  - Create resource
studio-cli artifacts bucket delete  - Delete resource
studio-cli artifacts bucket get  - Get resource
studio-cli artifacts bucket search  - Search resource
studio-cli artifacts bucket update  - Update resource
studio-cli artifacts bucket create
Create resource
Synopsis
Create resource
studio-cli artifacts bucket create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags":"{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--dest-path string        dest path of artifacts to create, example:
minio/$bucketName
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the

<!-- Page 65 -->

component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts bucket  - Artifacts bucket command group
studio-cli artifacts bucket delete
Delete resource

<!-- Page 66 -->

Synopsis
Delete resource
studio-cli artifacts bucket delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts bucket  - Artifacts bucket command group
studio-cli artifacts bucket get
Get resource
Synopsis
Get resource
studio-cli artifacts bucket get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn

<!-- Page 67 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts bucket  - Artifacts bucket command group
studio-cli artifacts bucket search
Search resource
Synopsis
Search resource
studio-cli artifacts bucket search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um

<!-- Page 68 -->

- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "bucket")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts bucket  - Artifacts bucket command group

<!-- Page 69 -->

studio-cli artifacts bucket update
Update resource
Synopsis
Update resource
studio-cli artifacts bucket update [flags]
Options
--dest-path string     dest path of artifacts to create
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change
-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 70 -->

## See Also
studio-cli artifacts bucket  - Artifacts bucket command group
studio-cli artifacts folder
Artifacts folder command group
Synopsis
Artifacts folder command group
Options
-h, --help        help for folder
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts  - Artifacts repository command group
studio-cli artifacts folder create  - Create resource
studio-cli artifacts folder delete  - Delete resource
studio-cli artifacts folder get  - Get resource
studio-cli artifacts folder search  - Search resource
studio-cli artifacts folder update  - Update resource
studio-cli artifacts folder create
Create resource

<!-- Page 71 -->

Synopsis
Create resource
studio-cli artifacts folder create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags":"{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--dest-path string        dest path of artifacts to create, example:
minio/$bucketName/$folderPath
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted

<!-- Page 72 -->

from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts folder  - Artifacts folder command group
studio-cli artifacts folder delete
Delete resource
Synopsis
Delete resource
studio-cli artifacts folder delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn

<!-- Page 73 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts folder  - Artifacts folder command group
studio-cli artifacts folder get
Get resource
Synopsis
Get resource
studio-cli artifacts folder get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts folder  - Artifacts folder command group

<!-- Page 74 -->

studio-cli artifacts folder search
Search resource
Synopsis
Search resource
studio-cli artifacts folder search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "folder")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state

<!-- Page 75 -->

- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts folder  - Artifacts folder command group
studio-cli artifacts folder update
Update resource
Synopsis
Update resource
studio-cli artifacts folder update [flags]
Options
--dest-path string     dest path of artifacts to create
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the

<!-- Page 76 -->

rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change
-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts folder  - Artifacts folder command group
studio-cli artifacts group
Group commands
Synopsis
Run a artifacts group sub command
Options
-h, --help   help for group

<!-- Page 77 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts  - Artifacts repository command group
studio-cli artifacts group assign  - Assign group access
studio-cli artifacts group revoke  - Revoke group access
studio-cli artifacts group assign
Assign group access
Synopsis
Assign a group access
studio-cli artifacts group assign [flags]
Options
-g, --group string      group name.
-h, --help              help for assign
-n, --name string       Resource name
-r, --rolename string   Role (viewer, tester, editor, or lead) (default "viewer")
-w, --wrrn string       wrrn of resource.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 78 -->

## See Also
studio-cli artifacts group  - Group commands
studio-cli artifacts group revoke
Revoke group access
Synopsis
Revoke a group access
studio-cli artifacts group revoke [flags]
Options
-g, --group string   group name.
-h, --help           help for revoke
-n, --name string    Resource name
-w, --wrrn string    wrrn of resource.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts group  - Group commands
studio-cli artifacts mc
Mc wrapper command, requires mc installed with studio-cli
Synopsis
Mc wrapper command, requires mc installed with studio-cli
studio-cli artifacts mc [flags]

<!-- Page 79 -->

Options
-h, --help   help for mc
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts  - Artifacts repository command group
studio-cli artifacts object
Artifacts object command group
Synopsis
Artifacts object command group
Options
-h, --help        help for object
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 80 -->

## See Also
studio-cli artifacts  - Artifacts repository command group
studio-cli artifacts object create  - Create resource
studio-cli artifacts object delete  - Delete resource
studio-cli artifacts object get  - Get resource
studio-cli artifacts object search  - Search resource
studio-cli artifacts object update  - Update resource
studio-cli artifacts object create
Create resource
Synopsis
Create resource
studio-cli artifacts object create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags":"{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--dest-path string        dest path of artifacts to create, example:
minio/$bucketName/$objectPath
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the

<!-- Page 81 -->

component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
--src-path string         src path locally of artifacts to copy from
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts object  - Artifacts object command group

<!-- Page 82 -->

studio-cli artifacts object delete
Delete resource
Synopsis
Delete resource
studio-cli artifacts object delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts object  - Artifacts object command group
studio-cli artifacts object get
Get resource
Synopsis
Get resource
studio-cli artifacts object get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string

<!-- Page 83 -->

-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts object  - Artifacts object command group
studio-cli artifacts object search
Search resource
Synopsis
Search resource
studio-cli artifacts object search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:

<!-- Page 84 -->

- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "object")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts object  - Artifacts object command group

<!-- Page 85 -->

studio-cli artifacts object update
Update resource
Synopsis
Update resource
studio-cli artifacts object update [flags]
Options
--dest-path string     dest path of artifacts to create
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change
--src-path string      src path of artifacts to copy from
-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 86 -->

## See Also
studio-cli artifacts object  - Artifacts object command group
studio-cli artifacts token
Artifacts token command group
Synopsis
Artifacts token command group.
Options
-h, --help   help for token
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts  - Artifacts repository command group
studio-cli artifacts token get  - Obtain a temporary token string for mc
studio-cli artifacts token get
Obtain a temporary token string for mc
Synopsis
Obtain a temporary token string for mc
studio-cli artifacts token get [flags]

<!-- Page 87 -->

Options
-h, --help          help for get
-s, --seconds int   Number of seconds for temporary token (default 3600)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli artifacts token  - Artifacts token command group
studio-cli completion
Generate the autocompletion script for the speciﬁed shell
Synopsis
Generate the autocompletion script for studio-cli for the speciﬁed shell. See each sub-command’s help
for details on how to use the generated script.
Options
-h, --help   help for completion
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 88 -->

## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli completion bash  - Generate the autocompletion script for bash
studio-cli completion ﬁsh  - Generate the autocompletion script for ﬁsh
studio-cli completion pow ershell  - Generate the autocompletion script for pow ershell
studio-cli completion zsh  - Generate the autocompletion script for zsh
studio-cli completion bash
Generate the autocompletion script for bash
Synopsis
Generate the autocompletion script for the bash shell.
This script depends on the ‘bash-completion’ package. If it is not installed already, you can install it via
your OS’s package manager.
To load completions in your current shell session:
source <(studio-cli completion bash)
To load completions for ev ery new session, execute once:
Linux:
studio-cli completion bash > /etc/bash_completion.d/studio-cli
macOS:
studio-cli completion bash > $(brew --prefix)/etc/bash_completion.d/studio-cli
You will need to start a new shell for this setup to take eﬀect.
studio-cli completion bash
Options
-h, --help              help for bash
--no-descriptions   disable completion descriptions
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses

<!-- Page 89 -->

--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli completion  - Generate the autocompletion script for the speciﬁed shell
studio-cli completion fish
Generate the autocompletion script for ﬁsh
Synopsis
Generate the autocompletion script for the ﬁsh shell.
To load completions in your current shell session:
studio-cli completion fish | source
To load completions for ev ery new session, execute once:
studio-cli completion fish > ~/.config/fish/completions/studio-cli.fish
You will need to start a new shell for this setup to take eﬀect.
studio-cli completion fish [flags]
Options
-h, --help              help for fish
--no-descriptions   disable completion descriptions
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 90 -->

## See Also
studio-cli completion  - Generate the autocompletion script for the speciﬁed shell
studio-cli completion powershell
Generate the autocompletion script for pow ershell
Synopsis
Generate the autocompletion script for pow ershell.
To load completions in your current shell session:
studio-cli completion powershell | Out-String | Invoke-Expression
To load completions for ev ery new session, add the output of the abov e command to your pow ershell
proﬁle.
studio-cli completion powershell [flags]
Options
-h, --help              help for powershell
--no-descriptions   disable completion descriptions
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli completion  - Generate the autocompletion script for the speciﬁed shell
studio-cli completion zsh
Generate the autocompletion script for zsh

<!-- Page 91 -->

Synopsis
Generate the autocompletion script for the zsh shell.
If shell completion is not already enabled in your environment you will need to enable it. Y ou can
execute the following once:
echo "autoload -U compinit; compinit" >> ~/.zshrc
To load completions in your current shell session:
source <(studio-cli completion zsh)
To load completions for ev ery new session, execute once:
Linux:
studio-cli completion zsh > "${fpath[1]}/_studio-cli"
macOS:
studio-cli completion zsh > $(brew --prefix)/share/zsh/site-functions/_studio-cli
You will need to start a new shell for this setup to take eﬀect.
studio-cli completion zsh [flags]
Options
-h, --help              help for zsh
--no-descriptions   disable completion descriptions
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli completion  - Generate the autocompletion script for the speciﬁed shell

<!-- Page 92 -->

studio-cli config
Manage user conﬁguration data
Synopsis
Manage user conﬁguration data.
Options
-h, --help        help for config
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli conﬁg debug  - Conﬁg debug command group
studio-cli conﬁg login  - Initiate or complete a remote login when using Azure AAD
studio-cli conﬁg logout  - Logout from wrstudio
studio-cli conﬁg proﬁle  - Conﬁg proﬁle command group
studio-cli conﬁg token  - Conﬁg token command group
studio-cli conﬁg update  - Update to latest binary by re-running the install
studio-cli conﬁg userdata  - Conﬁgure studio cli defaults
studio-cli config debug
Conﬁg debug command group
Synopsis
Conﬁg debug command group.

<!-- Page 93 -->

Options
-h, --help   help for debug
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg  - Manage user conﬁguration data
studio-cli conﬁg debug set  - Change debug settings
studio-cli config debug set
Change debug settings
Synopsis
Change debug settings.
studio-cli config debug set [flags]
Options
-d, --default             Restore debug setting to default
-e, --encoding string     Log encoding format, available: console|json
-h, --help                help for set
-l, --level string        Log level, available: debug|info|warn|error|panic|fatal
-o, --output string       Output studio response format, available: json|yaml
-p, --outputpath string   Log output path
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string

<!-- Page 94 -->

--non-interactive   Disable all interactive mode
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg debug  - Conﬁg debug command group
studio-cli config login
Initiate or complete a remote login when using Azure AAD
Synopsis
The login command only works with Azure AAD.
The Azure AAD is typically conﬁgured to use multi-factor authentication which requires a w eb
browser.
On a workstation with the w eb browser you can run the command:
studio-cli conﬁg login -i
The abov e command will print a command to run on the remote workstation. Y ou could either cut and
paste it or use ssh to run the printed command.
You could alternativ ely capture just the key and transmit it as a ﬁle using the -Q argument.
Local machine: studio-cli conﬁg login -Q -i > key_ﬁle scp key_ﬁle some_location:key_ﬁle
Remote machine: studio-cli conﬁg login -k $(cat key_ﬁle)
studio-cli config login [flags]
Options
-h, --help         help for login
-i, --init         Initialize the token, run this on a system with the web browser
-k, --key string   The key to save into the studio-cli config
-Q, --quiet        Only print the data string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 95 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg  - Manage user conﬁguration data
studio-cli config logout
Logout from wrstudio
Synopsis
Logout from wrstudio.
studio-cli config logout [flags]
Options
-a, --all               Remove all API keys as well as session data
-h, --help              help for logout
-r, --root string       Root name of studio
-u, --username string   My studio user name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg  - Manage user conﬁguration data
studio-cli config profile
Conﬁg proﬁle command group

<!-- Page 96 -->

Synopsis
Conﬁg proﬁle command group.
Options
-h, --help   help for profile
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg  - Manage user conﬁguration data
studio-cli conﬁg proﬁle add  - Add proﬁle
studio-cli conﬁg proﬁle delete  - Delete a proﬁle
studio-cli conﬁg proﬁle get  - Get a proﬁle
studio-cli conﬁg proﬁle list  - Show av ailable proﬁles
studio-cli conﬁg proﬁle set  - Set default proﬁle
studio-cli config profile add
Add proﬁle
Synopsis
Add proﬁle
studio-cli config profile add [flags]
Options
-e, --env string         Variables to set unless overridden by environment (eg. --
env="a=1;b=2)" (default "NONE")
-h, --help               help for add
-n, --name string        Profile name want to add, overwrite diff attributes if found one

<!-- Page 97 -->

existing
-r, --root string        Base name of WR Studio deployment, if not set try to read it env
-u, --user-name string   Set the default studio-cli user name, if not set try to read it
from env
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg proﬁle  - Conﬁg proﬁle command group
studio-cli config profile delete
Delete a proﬁle
Synopsis
Delete a proﬁle
studio-cli config profile delete [flags]
Options
-a, --all           Set studiocfg.yaml to empty
-h, --help          help for delete
-n, --name string   Profile name to be deleted
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 98 -->

## See Also
studio-cli conﬁg proﬁle  - Conﬁg proﬁle command group
studio-cli config profile get
Get a proﬁle
Synopsis
Get a proﬁle
studio-cli config profile get [flags]
Options
-h, --help          help for get
-n, --name string   Profile name for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg proﬁle  - Conﬁg proﬁle command group
studio-cli config profile list
Show av ailable proﬁles
Synopsis
Show av ailable proﬁles
studio-cli config profile list [flags]

<!-- Page 99 -->

Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg proﬁle  - Conﬁg proﬁle command group
studio-cli config profile set
Set default proﬁle
Synopsis
Set default proﬁle
studio-cli config profile set [flags]
Options
-h, --help          help for set
-n, --name string   Profile name for set
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 100 -->

## See Also
studio-cli conﬁg proﬁle  - Conﬁg proﬁle command group
studio-cli config token
Conﬁg token command group
Synopsis
Conﬁg token command group.
Options
-h, --help        help for token
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg  - Manage user conﬁguration data
studio-cli conﬁg token decode  - Display the data inside the jwt key
studio-cli conﬁg token get  - Get auth token of current user from remote serv er
studio-cli conﬁg token set  - Set auth token of current user
studio-cli conﬁg token show  - Show the current JWT key
studio-cli config token decode
Display the data inside the jwt key
Synopsis
Display the data inside the jwt key.

<!-- Page 101 -->

studio-cli config token decode [flags]
Options
-h, --help   help for decode
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg token  - Conﬁg token command group
studio-cli config token get
Get auth token of current user from remote serv er
Synopsis
Get auth token of current user from remote serv er.
studio-cli config token get [flags]
Options
-h, --help   help for get
-s, --show   Show the token and user id immediately
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 102 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg token  - Conﬁg token command group
studio-cli config token set
Set auth token of current user
Synopsis
Set auth token of current user.
studio-cli config token set [flags]
Options
-h, --help   help for set
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg token  - Conﬁg token command group
studio-cli config token show
Show the current JWT key
Synopsis
Show the current JWT key.

<!-- Page 103 -->

studio-cli config token show [flags]
Options
-h, --help   help for show
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg token  - Conﬁg token command group
studio-cli config update
Update to latest binary by re-running the install
Synopsis
Check the update repository and update to the latest v ersion of studio-cli. Y ou may set the
environment v ariable UPDA TE_URIBASE, or use the -u argument for force an update from another
location.
As long as bash is av ailable update command will download and run the installer.
studio-cli config update [flags]
Options
-b, --binary strings        update list of binaries only
-c, --canary                Install a canary build from [ devstar, latest ]
-f, --forceversion string   Select a specific version for the update (default latest)
-h, --help                  help for update
-i, --installdir string     Alternate location to perform the update
--quiet                 Do not print download status
-u, --url string            Override the default install URL
-v, --verbose               Verbose install
-y, --yes                   Answer yes to upgrade

<!-- Page 104 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg  - Manage user conﬁguration data
studio-cli config userdata
Conﬁgure studio cli defaults
Synopsis
When run without any arguments the studio-cli will open a conﬁguration dialog.
studio-cli config userdata [flags]
Options
-e, --env string          Variables to set unless overridden by environment (eg. --
env="a=1;b=2)" (default "NONE")
-h, --help                help for userdata
-l, --list                list the current values of deployment
-o, --outputtype string   Set the default output method [json|yaml]
-p, --password string     Set the default studio-cli password
-r, --root string         Base name of WR Studio deployment
-s, --savepw              How to save password: [auto|ssh-agent|ssh-local|no-save]
-u, --username string     Set the default studio-cli user name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 105 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli conﬁg  - Manage user conﬁguration data
studio-cli devreg
Device registry conﬁguration commands
Synopsis
Device registry conﬁguration commands
Options
-h, --help   help for devreg
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli devreg group  - Group commands
studio-cli devreg http  - Submit calls directly to the REST API
studio-cli devreg project  - Project manipulation commands
studio-cli devreg token  - Token commands
studio-cli devreg user  - User commands
studio-cli devreg group
Group commands

<!-- Page 106 -->

Synopsis
Run a registry group sub command
Options
-h, --help   help for group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg  - Device registry conﬁguration commands
studio-cli devreg group assign  - Assign group access
studio-cli devreg group revoke  - Revoke group access
studio-cli devreg group assign
Assign group access
Synopsis
Assign a group access
studio-cli devreg group assign [flags]
Options
-g, --group string      group name.
-h, --help              help for assign
-n, --name string       Resource name
-r, --rolename string   Role (viewer, tester, editor, or lead) (default "viewer")
-w, --wrrn string       wrrn of resource.

<!-- Page 107 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg group  - Group commands
studio-cli devreg group revoke
Revoke group access
Synopsis
Revoke a group access
studio-cli devreg group revoke [flags]
Options
-g, --group string   group name.
-h, --help           help for revoke
-n, --name string    Resource name
-w, --wrrn string    wrrn of resource.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg group  - Group commands

<!-- Page 108 -->

studio-cli devreg http
Submit calls directly to the REST API
Synopsis
The http subcommand works like a curl command where you specify the type of request and data via
the –data or –bodyﬁle argument if it is a POST or PUT call.
If the ﬁrst character of the -d/–data argument is an @ character, what follows will be used as a ﬁlename.
studio-cli devreg http [flags]
Options
-b, --bodyfile string   Data from file for a POST or PUT type call
-d, --data string       Data for a POST or PUT type call
-e, --endpoint string   end point (e.g. /users/current)
-h, --help              help for http
-q, --jq string         jq query string
-t, --type              http request type, [DELETE|GET|HEAD|POST|PUT]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg  - Device registry conﬁguration commands
studio-cli devreg project
Project manipulation commands
Synopsis
Project manipulation commands

<!-- Page 109 -->

Options
-h, --help        help for project
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg  - Device registry conﬁguration commands
studio-cli devreg project create  - Create a container registry project
studio-cli devreg project delete  - Delete resource
studio-cli devreg project get  - Get resource
studio-cli devreg project group  - Group commands
studio-cli devreg project list  - List container registry projects
studio-cli devreg project members  - Members commands
studio-cli devreg project repo  - Repo commands
studio-cli devreg project robot  - Robot commands
studio-cli devreg project search  - Search resource
studio-cli devreg project update  - Update a container registry project
studio-cli devreg project user  - User commands
studio-cli devreg project create
Create a container registry project
Synopsis
Create a container registry project
studio-cli devreg project create [flags]

<!-- Page 110 -->

Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags": "{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-name string       user group name
-h, --help                    help for create
-q, --jq string               jq query string
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
--private                 Make the project private when creating
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is

<!-- Page 111 -->

true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project delete
Delete resource
Synopsis
Delete resource
studio-cli devreg project delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 112 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project get
Get resource
Synopsis
Get resource
studio-cli devreg project get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project group
Group commands

<!-- Page 113 -->

Synopsis
Group commands
Options
-h, --help   help for group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project group add  - Add group to project
studio-cli devreg project group delete  - Delete group from container registry project
studio-cli devreg project group update  - Update group role in project
studio-cli devreg project group add
Add group to project
Synopsis
Add an additional group to the project with a role.
The role mappings are [WR Studio role == Harbor Role (RoleId # in Harbor)]:
viewer == Guest (3)
tester == Developer (2)
editor == Maintainer (4)
lead   == Project Admin (1)
studio-cli devreg project group add [flags]

<!-- Page 114 -->

Options
-g, --group string   group name to add
-h, --help           help for add
-n, --name string    project name or project ID
-r, --role           Role to add to group [viewer|tester|editor|lead]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project group  - Group commands
studio-cli devreg project group delete
Delete group from container registry project
Synopsis
Delete a group by group name or the group ID for the speciﬁed container project.
studio-cli devreg project group delete [flags]
Options
-g, --group string   The group name or group ID to remove from project
-h, --help           help for delete
-n, --name string    the project name or ID
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 115 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project group  - Group commands
studio-cli devreg project group update
Update group role in project
Synopsis
Update a group’s role in the speciﬁed project.
The role mappings are [WR Studio role == Harbor Role (RoleId # in Harbor )]: view er == Guest (3) tester
== Dev eloper (2) editor == Maintainer (4) lead == Project Admin (1)
studio-cli devreg project group update [flags]
Options
-g, --group string   group name to modify
-h, --help           help for update
-n, --name string    project name or project ID
-r, --role           Role to add to group [viewer|tester|editor|lead]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project group  - Group commands
studio-cli devreg project list
List container registry projects

<!-- Page 116 -->

Synopsis
List container registry projects
studio-cli devreg project list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project members
Members commands
Synopsis
Members commands.
Options
-h, --help   help for members
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 117 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project members list  - List container registry project members
studio-cli devreg project members list
List container registry project members
Synopsis
List container registry project members
studio-cli devreg project members list [flags]
Options
-a, --all            display all or not,default: false
-h, --help           help for list
-n, --name string    the project name or ID
--page int       the page number (default 1)
--pagesize int   the page size (default 10)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project members  - Members commands
studio-cli devreg project repo
Repo commands

<!-- Page 118 -->

Synopsis
Repo commands.
Options
-h, --help   help for repo
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project repo artifacts  - List artifacts in container repo
studio-cli devreg project repo info  - Display container repository information
studio-cli devreg project repo list  - List repositories in project
studio-cli devreg project repo remov e - Remov e container repository from project
studio-cli devreg project repo scan  - Initiate a repository scan on a speciﬁc tag or reference
studio-cli devreg project repo tag  - Tag an artifact in a container repo
studio-cli devreg project repo artifacts
List artifacts in container repo
Synopsis
List artifacts in container repo.
To query for tags you can use: –jq ‘[.[]|select(.tags)|{“digest”:.digest,”tags”:[.tags[].name]}]’ To query
for vunlerability report URL –jq ‘.
[]|select(.tags)|select(.tags[]|.name==”latest”)|.addition_links.vulnerabilities.href’ –raw Th result of the
query abov e can be used with the “http -t GET -e RESUL T”
studio-cli devreg project repo artifacts [flags]

<!-- Page 119 -->

Options
-h, --help          help for artifacts
-n, --name string   Name of project
-r, --repo string   Name of repository
-t, --tag string    Search for artifact by tag name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project repo  - Repo commands
studio-cli devreg project repo info
Display container repository information
Synopsis
Display container repository information
studio-cli devreg project repo info [flags]
Options
-h, --help          help for info
-n, --name string   Name of project
-r, --repo string   Name of repository
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 120 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project repo  - Repo commands
studio-cli devreg project repo list
List repositories in project
Synopsis
List repositories in project
studio-cli devreg project repo list [flags]
Options
-h, --help          help for list
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project repo  - Repo commands
studio-cli devreg project repo remove
Remov e container repository from project
Synopsis
Remov e container repository from project

<!-- Page 121 -->

studio-cli devreg project repo remove [flags]
Options
-h, --help          help for remove
-n, --name string   Name of project
-r, --repo string   Name of repository
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project repo  - Repo commands
studio-cli devreg project repo scan
Initiate a repository scan on a speciﬁc tag or reference
Synopsis
Initiate a repository scan on a speciﬁc tag or reference
studio-cli devreg project repo scan [flags]
Options
-h, --help          help for scan
-n, --name string   Name of project
-r, --repo string   Name of repository
-t, --tag string    Name of repository
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 122 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project repo  - Repo commands
studio-cli devreg project repo tag
Tag an artifact in a container repo
Synopsis
Add a tag, delete a tag or display tags for a giv en artifact in a container repo.
When run without a tag argument the tags are display ed. When run with a tag argument a tag is
added. If the delete argument speciﬁed along with a tag argument the tag will be remov ed from the
artifact.
The artifact hash can optionally be omitted if deleting a tag, which will cause a search for the artifact to
remov e the tag.
studio-cli devreg project repo tag [flags]
Options
-a, --artifact string   Artifact digest hash (found from repoartifacts command)
-d, --delete            Delete specified tag
-h, --help              help for tag
-n, --name string       Name of project
-r, --repo string       Name of repository
-t, --tag strings       Artifact digest hash (found from repoartifacts command)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 123 -->

## See Also
studio-cli devreg project repo  - Repo commands
studio-cli devreg project robot
Robot commands
Synopsis
Robot commands.
Options
-h, --help   help for robot
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project robot create  - Create robot account
studio-cli devreg project robot list  - List robot accounts
studio-cli devreg project robot remov e - Remov e robot account
studio-cli devreg project robot create
Create robot account
Synopsis
Create robot account
studio-cli devreg project robot create [flags]

<!-- Page 124 -->

Options
-h, --help               help for create
-n, --name string        Name of project
-r, --robotname string   Name of robot
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project robot  - Robot commands
studio-cli devreg project robot list
List robot accounts
Synopsis
List robot accounts
studio-cli devreg project robot list [flags]
Options
-h, --help          help for list
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 125 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project robot  - Robot commands
studio-cli devreg project robot remove
Remov e robot account
Synopsis
Remov e robot account
studio-cli devreg project robot remove [flags]
Options
-h, --help               help for remove
-n, --name string        Name of project
-r, --robotname string   Name of robot
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project robot  - Robot commands
studio-cli devreg project search
Search resource

<!-- Page 126 -->

Synopsis
Search resource
studio-cli devreg project search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-q, --jq string               jq query string
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "project")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use

<!-- Page 127 -->

- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project update
Update a container registry project
Synopsis
Update a container registry project
studio-cli devreg project update [flags]
Options
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change

<!-- Page 128 -->

-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project user
User commands
Synopsis
User commands
Options
-h, --help   help for user
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 129 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project  - Project manipulation commands
studio-cli devreg project user add  - Add user to project
studio-cli devreg project user delete  - Delete user from container registry project
studio-cli devreg project user update  - Update user role in project
studio-cli devreg project user add
Add user to project
Synopsis
Add an additional user to the project with a role.
The role mappings are [WR Studio role == Harbor Role (RoleId # in Harbor )]: view er == Guest (3) tester
== Dev eloper (2) editor == Maintainer (4) lead == Project Admin (1)
studio-cli devreg project user add [flags]
Options
-h, --help          help for add
-n, --name string   project name or project ID
-r, --role          Role to add to group [viewer|tester|editor|lead]
-u, --user string   user name to add
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 130 -->

## See Also
studio-cli devreg project user  - User commands
studio-cli devreg project user delete
Delete user from container registry project
Synopsis
Delete a user by user name or the member ID for the speciﬁed container project.
studio-cli devreg project user delete [flags]
Options
-h, --help          help for delete
-n, --name string   the project name or ID
-u, --user string   The user name or member ID to remove from project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project user  - User commands
studio-cli devreg project user update
Update user role in project
Synopsis
Update a user ’s role in the speciﬁed project.
The role mappings are [WR Studio role == Harbor Role (RoleId # in Harbor )]: view er == Guest (3) tester
== Dev eloper (2) editor == Maintainer (4) lead == Project Admin (1)

<!-- Page 131 -->

studio-cli devreg project user update [flags]
Options
-h, --help          help for update
-n, --name string   project name or project ID
-r, --role          Role to add to group [viewer|tester|editor|lead]
-u, --user string   user name to modify
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg project user  - User commands
studio-cli devreg token
Token commands
Synopsis
Token commands
Options
-h, --help        help for token
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 132 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg  - Device registry conﬁguration commands
studio-cli devreg token get  - Get cli access token (same as User Proﬁle in the W eb UI)
studio-cli devreg token get
Get cli access token (same as User Proﬁle in the W eb UI)
Synopsis
Get cli access token (same as User Proﬁle in the W eb UI)
studio-cli devreg token get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg token  - Token commands
studio-cli devreg user
User commands
Synopsis
User commands

<!-- Page 133 -->

Options
-h, --help        help for user
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg  - Device registry conﬁguration commands
studio-cli devreg user assign-admin  - Assign user admin role
studio-cli devreg user info  - List container registry current user account details
studio-cli devreg user list  - List container registry users
studio-cli devreg user unassign-admin  - Remov e user admin role
studio-cli devreg user assign-admin
Assign user admin role
Synopsis
Add the harbor administrator role to a user by name or ID.
studio-cli devreg user assign-admin [flags]
Options
-h, --help          help for assign-admin
-n, --name string   user name or ID
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 134 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg user  - User commands
studio-cli devreg user info
List container registry current user account details
Synopsis
List container registry current user account details
studio-cli devreg user info [flags]
Options
-h, --help   help for info
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg user  - User commands
studio-cli devreg user list
List container registry users

<!-- Page 135 -->

Synopsis
List container registry users
studio-cli devreg user list [flags]
Options
-a, --all            display all or not,default: false
-h, --help           help for list
--page int       the page number (default 1)
--pagesize int   the page size (default 10)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg user  - User commands
studio-cli devreg user unassign-admin
Remov e user admin role
Synopsis
Remov e the harbor administrator role from a user by name or ID.
studio-cli devreg user unassign-admin [flags]
Options
-h, --help          help for unassign-admin
-n, --name string   user name or ID

<!-- Page 136 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli devreg user  - User commands
studio-cli dfl
Digital Feedback Loop
Synopsis
Digital Feedback Loop”
Options
-h, --help        help for dfl
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 137 -->

## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli dﬂ certiﬁcate  - Certiﬁcate manager
studio-cli dﬂ coldpath  - Coldpath manager
studio-cli dﬂ command  - Command manager
studio-cli dﬂ commondimensions  - Commondimensions manager
studio-cli dﬂ device  - Device manager
studio-cli dﬂ device-type  - DeviceType manager
studio-cli dﬂ endpoint  - Endpoint manager
studio-cli dﬂ ﬁle-transfer  - File-transfer manager
studio-cli dﬂ hotpath  - Hotpath manager
studio-cli dﬂ log  - Log manager
studio-cli dﬂ schema  - Schema manager
studio-cli dﬂ statistic  - Statistic manager
studio-cli dﬂ threshold  - Threshold manager
studio-cli dfl certificate
Certiﬁcate manager
Synopsis
Certiﬁcate manager
Options
-h, --help        help for certificate
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 138 -->

## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ certiﬁcate get  - Finds the list of all certiﬁcates
studio-cli dﬂ certiﬁcate update  - Updates the certiﬁcate status
studio-cli dfl certificate get
Finds the list of all certiﬁcates
Synopsis
Returns the list of all certiﬁcates of speciﬁc device_name
studio-cli dfl certificate get [flags]
Options
-d, --device-name string   The device name to check
-h, --help                 help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ certiﬁcate  - Certiﬁcate manager
studio-cli dfl certificate update
Updates the certiﬁcate status

<!-- Page 139 -->

Synopsis
Updates the Status to ACTIVE, INACTIVE, REVOKED for a single or list of certiﬁcates for a speciﬁc
device name DFL
studio-cli dfl certificate update [flags]
Options
-a, --action string             ACTIVE, INACTIVE, REVOKED
-i, --certificate-ids strings   certificate ids
-d, --device-name string        The device name to check
-h, --help                      help for update
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ certiﬁcate  - Certiﬁcate manager
studio-cli dfl coldpath
Coldpath manager
Synopsis
Coldpath manager
Options
-h, --help        help for coldpath
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 140 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ coldpath get  - Finds the cold path data
studio-cli dfl coldpath get
Finds the cold path data
Synopsis
Retriev e the stored data in the cold path
studio-cli dfl coldpath get [flags]
Options
-d, --device-name string            The device name (default "all")
-h, --help                          help for get
--limit int32                   The limit of data retrieved (default 10)
--query-since string            The last time to check, in format ago(#(h/m))
--query-until string            The recent time to check, in format ago(#(h/m)) or
now()
-t, --table string                  The table to retrieved the data (default
"wr_dfl_coldpath_glue_linux")
-u, --user-specified-field string   The specific information to search
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 141 -->

## See Also
studio-cli dﬂ coldpath  - Coldpath manager
studio-cli dfl command
Command manager
Synopsis
Command manager
Options
-h, --help   help for command
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ command get  - Get all the commands logs
studio-cli dﬂ command send  - Send a command
studio-cli dfl command get
Get all the commands logs
Synopsis
Get all the commands logs with the supplied parameters for the request
studio-cli dfl command get [flags]

<!-- Page 142 -->

Options
-c, --commands string             The request type, request or response (default
"request")
-d, --device-name string          The requested device name (default "all")
-h, --help                        help for get
-l, --last-evaluated-key string   The value returned in the last call (default "None")
--limit int32                 Limit of items to evaluate (default 10)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ command  - Command manager
studio-cli dfl command send
Send a command
Synopsis
Send a command with the supplied parameters for the device.
Format: studio-cli dfl command send -o <operation_cmd> -t <device_type> -i <device_name> -p
<stringified_args_json> -a <os_or_app_name>
studio-cli dfl command send [flags]
Options
-a, --application-name string   The command application type such as an OS or application
name
-p, --args string               The args of command, like "{"--name": "wireshark", "--
version": "latest"}"
-t, --device-type string        The device type to check such as linux or vxworks
-h, --help                      help for send
-o, --operation string          The operation of command, like "install"
-i, --topic string              The topic of command takes in the devicename

<!-- Page 143 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ command  - Command manager
studio-cli dfl commondimensions
Commondimensions manager
Synopsis
Commondimensions manager
Options
-h, --help   help for commondimensions
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ commondimensions add  - Add new common data dimensions
studio-cli dﬂ commondimensions get  - Get the existing common data dimensions

<!-- Page 144 -->

studio-cli dfl commondimensions add
Add new common data dimensions
Synopsis
Add new common data dimensions
To add multiple:
Format: studio-cli dfl commondimensions add -p
"common_dimension_name1=description1" -p "common_dimension_name2=description2" -t
device_type
To add single:
Format: studio-cli dfl commondimensions add -p "common_dimension_name=description"
-t device_type
studio-cli dfl commondimensions add [flags]
Options
-t, --device-type string               The requested device_type such as linux or vxworks
-p, --dimensions-payload stringArray   The requested dimensions param payload
Example: --dimensions-payload 'name1=description1' --dimensions-payload
'name2=description2'
-h, --help                             help for add
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ commondimensions  - Commondimensions manager
studio-cli dfl commondimensions get
Get the existing common data dimensions

<!-- Page 145 -->

Synopsis
Get the existing common data dimensions
studio-cli dfl commondimensions get [flags]
Options
-d, --device-type string   The requested device type
-h, --help                 help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ commondimensions  - Commondimensions manager
studio-cli dfl device
Device manager
Synopsis
Device manager
Options
-h, --help   help for device
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode

<!-- Page 146 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ device create  - Register new device
studio-cli dﬂ device delete  - Delete a device
studio-cli dﬂ device get  - Get the device information
studio-cli dﬂ device list  - Finds devices information
studio-cli dﬂ device update  - Update the device with secret zero ﬂag
studio-cli dfl device-type
DeviceType manager
Synopsis
DeviceType manager
Options
-h, --help        help for device-type
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ device-type get  - Get all the deviceTypes logs

<!-- Page 147 -->

studio-cli dfl device-type get
Get all the deviceTypes logs
Synopsis
Get all the deviceTypes logs with the supplied parameters for the request
studio-cli dfl device-type get [flags]
Options
-t, --device-type string          The device type to check (default "None")
-h, --help                        help for get
-k, --last-evaluated-key string   The last device to start checking in the format
'primary:vxworks' (default "None")
-l, --limit int32                 The numbers of items to return (default 10)
-o, --operation string            To check for a single or specific or all device types
(default "all")
-v, --schema-id-version string    To check for a single or specific schema id version
(default "None")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ device-type  - DeviceType manager
studio-cli dfl device create
Register new device
Synopsis
Register new device
studio-cli dfl device create [flags]

<!-- Page 148 -->

Options
-d, --devices strings   The device(s) to the registered. (Format e.g.,
devicename:devicetype)
-h, --help              help for create
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ device  - Device manager
studio-cli dfl device delete
Delete a device
Synopsis
Delete a device
CMD Format - Delete one: studio-cli dfl device delete -n <device-name>
CMD Format - Delete multiple: studio-cli dfl device delete -n "<device-name1>,<device-
name2>,<device-name3>"
studio-cli dfl device delete [flags]
Options
-n, --device-name strings   The device(s) name to delete
-h, --help                  help for delete
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses

<!-- Page 149 -->

--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ device  - Device manager
studio-cli dfl device get
Get the device information
Synopsis
Get the device information
studio-cli dfl device get [flags]
Options
-d, --device-name string   The requested device name
-h, --help                 help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ device  - Device manager
studio-cli dfl device list
Finds devices information

<!-- Page 150 -->

Synopsis
Finds devices information
studio-cli dfl device list [flags]
Options
-n, --device-name string          The requested device name (default "None")
-s, --device-status string        Status of the device whether it is connected or not
(default "None")
-h, --help                        help for list
-k, --last-evaluated-key string   The value returned in the last call (default "None")
-l, --limit int32                 Limit of items to evaluate (default 10)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ device  - Device manager
studio-cli dfl device update
Update the device with secret zero ﬂag
Synopsis
Update the device with secret zero ﬂag
studio-cli dfl device update [flags]
Options
-f, --flag string   secret_zero_enabled: true/false (default "true")
-h, --help          help for update
-n, --name string   Name Of Device

<!-- Page 151 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ device  - Device manager
studio-cli dfl endpoint
Endpoint manager
Synopsis
Endpoint manager
Options
-h, --help        help for endpoint
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ endpoint get  - Finds the list of endpoints

<!-- Page 152 -->

studio-cli dfl endpoint get
Finds the list of endpoints
Synopsis
Finds the list of endpoints - mqtt_endpoint, secret_zero_endpoint and secret_key
studio-cli dfl endpoint get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ endpoint  - Endpoint manager
studio-cli dfl file-transfer
File-transfer manager
Synopsis
File-transfer manager
Options
-h, --help        help for file-transfer
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 153 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ ﬁle-transfer create  - Create a ﬁle transfer job on DFL
studio-cli dﬂ ﬁle-transfer delete  - Delete a job on DFL
studio-cli dﬂ ﬁle-transfer get  - Get information about ﬁles registered in DFL
studio-cli dﬂ ﬁle-transfer update  - Trigger a job on DFL
studio-cli dfl file-transfer create
Create a ﬁle transfer job on DFL
Synopsis
Create a ﬁle transfer job on DFL
studio-cli dfl file-transfer create [flags]
Options
-c, --command-id string    The command id for download command
--description string   description for upload
-d, --device-name string   The device name for download or upload
-h, --help                 help for create
-n, --name string          File name for upload
-o, --operation string     operation for upload
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 154 -->

## See Also
studio-cli dﬂ ﬁle-transfer  - File-transfer manager
studio-cli dfl file-transfer delete
Delete a job on DFL
Synopsis
Delete a job on DFL
studio-cli dfl file-transfer delete [flags]
Options
-h, --help                  help for delete
-r, --records stringArray   The requested device name and commandId
[{\"device_name\":\"device1\",\"commandId\":\"xxx\"}]

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ ﬁle-transfer  - File-transfer manager
studio-cli dfl file-transfer get
Get information about ﬁles registered in DFL
Synopsis
Get information about ﬁles registered in DFL

<!-- Page 155 -->

studio-cli dfl file-transfer get [flags]
Options
-c, --command-id string           Specifies the commandId
-d, --device-name string          Specifies the requested device name (default "all")
-h, --help                        help for get
-l, --last-evaluated-key string   The requested device name
--limit int32                 The number of entries to be retrieved (default 10)
-n, --name string                 File name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ ﬁle-transfer  - File-transfer manager
studio-cli dfl file-transfer update
Trigger a job on DFL
Synopsis
Trigger a job on DFL
studio-cli dfl file-transfer update [flags]
Options
-c, --command-id string    command id
-d, --device-name string   The device name
-h, --help                 help for update
-s, --status string        status

<!-- Page 156 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ ﬁle-transfer  - File-transfer manager
studio-cli dfl hotpath
Hotpath manager
Synopsis
Hotpath manager
Options
-h, --help        help for hotpath
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ hotpath get  - Get all the hotpaths logs

<!-- Page 157 -->

studio-cli dfl hotpath get
Get all the hotpaths logs
Synopsis
Get all the hotpaths logs with the supplied parameters for the request
studio-cli dfl hotpath get [flags]
Options
-d, --device-name string            The device name (default "all")
-h, --help                          help for get
--limit int32                   The limit of data retrieved (default 10)
-m, --measure-type string           The type of data to retrieved (default "bigint")
--query-since string            The last time to check, in format ago(#(h/m))
(default "ago(1h)")
--query-until string            The recent time to check, in format ago(#(h/m)) or
now() (default "now()")
-t, --table string                  The table to retrieved the data (default
"wr_dfl_real_time_linux_default")
-u, --user-specified-field string   The specific information to search (default
"RamTotal")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ hotpath  - Hotpath manager
studio-cli dfl log
Log manager
Synopsis
Log manager

<!-- Page 158 -->

Options
-h, --help        help for log
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ log get  - Finds the logs related to a devices
studio-cli dfl log get
Finds the logs related to a devices
Synopsis
Logs about diﬀerent devices in the DFL
studio-cli dfl log get [flags]
Options
-d, --device-name string          The device name to check (default "all")
-e, --event-type string           The event type to check ex. schema, os schema,
threshold (default "all")
-h, --help                        help for get
-l, --last-evaluated-key string   The last device to start checking (default "None")
--limit int32                 The numbers of items to return (default 10)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override

<!-- Page 159 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ log  - Log manager
studio-cli dfl schema
Schema manager
Synopsis
Schema manager
Options
-h, --help        help for schema
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ schema get  - Finds the devices schema
studio-cli dfl schema get
Finds the devices schema
Synopsis
Information about diﬀerent devices in the DFL

<!-- Page 160 -->

studio-cli dfl schema get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ schema  - Schema manager
studio-cli dfl statistic
Statistic manager
Synopsis
Statistic manager
Options
-h, --help        help for statistic
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 161 -->

## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ statistic get  - Finds the devices statistics
studio-cli dfl statistic get
Finds the devices statistics
Synopsis
Information about diﬀerent devices in the DFL
studio-cli dfl statistic get [flags]
Options
-h, --help               help for get
-o, --operation string   Operation to execute, it can be empty or realtime (default
"realtime")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ statistic  - Statistic manager
studio-cli dfl threshold
Threshold manager
Synopsis
Threshold manager

<!-- Page 162 -->

Options
-h, --help        help for threshold
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ  - Digital Feedback Loop
studio-cli dﬂ threshold create  - Update the thresholds information
studio-cli dﬂ threshold delete  - Deletes a list of thresholds for IDs
studio-cli dﬂ threshold get  - Finds the devices thresholds
studio-cli dﬂ threshold update  - Update the thresholds information
studio-cli dfl threshold create
Update the thresholds information
Synopsis
Updates a single or list of thresholds for speciﬁc device types and names.
studio-cli dfl threshold create [flags]
Examples
studio-cli dfl threshold create -d "description" --device-type "device-type" -i "interval"
-n "name" -o "operator" -s "severity" -v "value" -e "email1,email2"
Options
-d, --description string
--device-type string
-e, --emails strings       For example: --emails="email1,email2"
-h, --help                 help for create

<!-- Page 163 -->

-i, --interval string
-n, --name string
-o, --operator string
-s, --severity string
-v, --value string          (default "0")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ threshold  - Threshold manager
studio-cli dfl threshold delete
Deletes a list of thresholds for IDs
Synopsis
Deletes a list of thresholds for speciﬁc IDs
studio-cli dfl threshold delete [flags]
Examples
studio-cli dfl threshold delete -d "device_type" -i "id1, id2..."
Options
-d, --device-type string   The device type
-h, --help                 help for delete
-i, --ids strings          The IDs of the thresholds. For example: --ids="id1,id2"
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses

<!-- Page 164 -->

--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ threshold  - Threshold manager
studio-cli dfl threshold get
Finds the devices thresholds
Synopsis
Information about diﬀerent devices in the DFL
studio-cli dfl threshold get [flags]
Options
-d, --device-type string   The device type (default "all")
-h, --help                 help for get
-n, --name string          The name of the threshold, it is optional
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ threshold  - Threshold manager
studio-cli dfl threshold update
Update the thresholds information

<!-- Page 165 -->

Synopsis
Updates a single threshold for speciﬁc device type and name.
studio-cli dfl threshold update [flags]
Examples
studio-cli dfl threshold update -d "description" --device-type "device-type" -i "interval"
-n "name" -o "operator" -s "severity" -v "value" -e "email1,email2"
Options
-d, --description string
--device-type string
-e, --emails strings       For example: --emails="email1,email2"
-h, --help                 help for update
-i, --interval string
-n, --name string
-o, --operator string
-s, --severity string
-v, --value string          (default "0")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli dﬂ threshold  - Threshold manager
studio-cli gendoc
Generate command reference document
Synopsis
Generate command reference document.

<!-- Page 166 -->

studio-cli gendoc [flags]
Options
-h, --help          help for gendoc
-p, --path string   existing and writable directory to store command document (default
"./docs")
-t, --type string   document type, must be one of Markdown|Man|ReST|Yaml. Man=Manual
Page, ReST=ReStructured Text (default "Markdown")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli gojq
Act as gojq which can read/write/modify json and y aml
Synopsis
Act as gojq which can read/write/modify json and y aml
studio-cli gojq [flags]
Options
-h, --help   help for gojq
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 167 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli jenkins
Conﬁg jenkins
Synopsis
Conﬁg jenkins
Options
-h, --help        help for jenkins
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli jenkins conﬁg  - Conﬁg commands
studio-cli jenkins folder  - Folder commands
studio-cli jenkins freesty le-project  - freesty le-project commands
studio-cli jenkins group  - Group commands
studio-cli jenkins pipeline  - pipeline commands
studio-cli jenkins token  - Token commands

<!-- Page 168 -->

studio-cli jenkins config
Conﬁg commands
Synopsis
Conﬁg commands
Options
-h, --help   help for config
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins  - Conﬁg jenkins
studio-cli jenkins conﬁg edit  - Edit jenkins item conﬁguration
studio-cli jenkins config edit
Edit jenkins item conﬁguration
Synopsis
Get, put or edit a Jenkins folder, or item using a jenkins xml conﬁguration ﬁle.
studio-cli jenkins config edit [flags]
Options
--get string    Save the jenkins item configuration to a file name
-h, --help          help for edit
-i, --item string   Specify item path e.g. /wrlinux/wrlinux-USP-build
--put string    Replace the jenkins item configuration with the specified file
-u, --url string    Specify fully qualified jenkins URL

<!-- Page 169 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins conﬁg  - Conﬁg commands
studio-cli jenkins folder
Folder commands
Synopsis
FolderJENKINS commands
Options
-h, --help        help for folder
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 170 -->

## See Also
studio-cli jenkins  - Conﬁg jenkins
studio-cli jenkins folder create  - Create a Jenkins item from a template with RBAC default
permissions
studio-cli jenkins folder delete  - Delete resource
studio-cli jenkins folder get  - Get resource
studio-cli jenkins folder search  - Search resource
studio-cli jenkins folder update  - Add a group with a predeﬁned role to a jenkins conﬁguration item
studio-cli jenkins folder create
Create a Jenkins item from a template with RBAC default permissions
Synopsis
Create a Jenkins item such as a Jenkins project.Each created item uses a pre-deﬁned permissions
template applied.
studio-cli jenkins folder create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags": "{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the

<!-- Page 171 -->

component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins folder  - Folder commands
studio-cli jenkins folder delete
Delete resource

<!-- Page 172 -->

Synopsis
Delete resource
studio-cli jenkins folder delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins folder  - Folder commands
studio-cli jenkins folder get
Get resource
Synopsis
Get resource
studio-cli jenkins folder get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn

<!-- Page 173 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins folder  - Folder commands
studio-cli jenkins folder search
Search resource
Synopsis
Search resource
studio-cli jenkins folder search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um

<!-- Page 174 -->

- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "folder")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins folder  - Folder commands

<!-- Page 175 -->

studio-cli jenkins folder update
Add a group with a predeﬁned role to a jenkins conﬁguration item
Synopsis
Add a group with a predeﬁned role to a jenkins conﬁguration item. Y ou can choose from the roles of :
lead, dev eloper, tester, view er
studio-cli jenkins folder update [flags]
Options
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change
-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 176 -->

## See Also
studio-cli jenkins folder  - Folder commands
studio-cli jenkins freestyle-project
freesty le-project commands
Synopsis
freesty le-project commands
Options
-h, --help        help for freestyle-project
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins  - Conﬁg jenkins
studio-cli jenkins freesty le-project build  - Build commands
studio-cli jenkins freesty le-project create  - Create a Jenkins item from a template with RBAC default
permissions
studio-cli jenkins freesty le-project delete  - Delete resource
studio-cli jenkins freesty le-project get  - Get resource
studio-cli jenkins freesty le-project search  - Search resource
studio-cli jenkins freesty le-project update  - Add a group with a predeﬁned role to a jenkins
conﬁguration item
studio-cli jenkins freestyle-project build
Build commands

<!-- Page 177 -->

Synopsis
Build commands
Options
-h, --help   help for build
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins freesty le-project  - freesty le-project commands
studio-cli jenkins freesty le-project build start  - Start build a Jenkins Job
studio-cli jenkins freesty le-project build status  - Build status of a Jenkins Job
studio-cli jenkins freesty le-project build tail  - Print console log of jenkins build
studio-cli jenkins freestyle-project build start
Start build a Jenkins Job
Synopsis
Start build a Jenkins Job
studio-cli jenkins freestyle-project build start [flags]
Options
-f, --file string          Provide Params in json/yaml format via a file
-h, --help                 help for start
-j, --job string           Specify jenkins job path e.g. /wrlinux/wrlinux-USP-build
-p, --params stringArray   Params (e.g. --param param1=value1)

<!-- Page 178 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins freesty le-project build  - Build commands
studio-cli jenkins freestyle-project build status
Build status of a Jenkins Job
Synopsis
Build status of a Jenkins Job
studio-cli jenkins freestyle-project build status [flags]
Options
-b, --build-id string   Specify jenkins build id
-h, --help              help for status
-j, --job string        Specify jenkins job path e.g. /wrlinux/wrlinux-USP-build
--quiet             When waiting for the jenkins builds to complete do not emit
status
-s, --sleep int         Sleep poll interval seconds until jenkins build is finished, must
be greater than 0 (default 10)
-w, --wait              Wait for the jenkins build to complete
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 179 -->

## See Also
studio-cli jenkins freesty le-project build  - Build commands
studio-cli jenkins freestyle-project build tail
Print console log of jenkins build
Synopsis
Print console log of jenkins build
studio-cli jenkins freestyle-project build tail [flags]
Options
-h, --help         help for tail
-j, --job string   Tail by job name e.g. /wrlinux/wrlinux-USP-build/1
-n, --nowait       Do not wait for log to complete
-s, --sleep int    Sleep poll interval in seconds (default: 2) (default 2)
-u, --url string   Specify fully qualified jenkins URL
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins freesty le-project build  - Build commands
studio-cli jenkins freestyle-project create
Create a Jenkins item from a template with RBAC default permissions
Synopsis
Create a Jenkins item such as a Jenkins project.Each created item uses a pre-deﬁned permissions
template applied.

<!-- Page 180 -->

studio-cli jenkins freestyle-project create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags": "{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]

<!-- Page 181 -->

--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins freesty le-project  - freesty le-project commands
studio-cli jenkins freestyle-project delete
Delete resource
Synopsis
Delete resource
studio-cli jenkins freestyle-project delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override

<!-- Page 182 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins freesty le-project  - freesty le-project commands
studio-cli jenkins freestyle-project get
Get resource
Synopsis
Get resource
studio-cli jenkins freestyle-project get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins freesty le-project  - freesty le-project commands
studio-cli jenkins freestyle-project search
Search resource

<!-- Page 183 -->

Synopsis
Search resource
studio-cli jenkins freestyle-project search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "folder")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first

<!-- Page 184 -->

time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins freesty le-project  - freesty le-project commands
studio-cli jenkins freestyle-project update
Add a group with a predeﬁned role to a jenkins conﬁguration item
Synopsis
Add a group with a predeﬁned role to a jenkins conﬁguration item. Y ou can choose from the roles of :
lead, dev eloper, tester, view er
studio-cli jenkins freestyle-project update [flags]
Options
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.

<!-- Page 185 -->

this is used when managing the resources outside of
Studio where the end user will create the resource and change
-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins freesty le-project  - freesty le-project commands
studio-cli jenkins group
Group commands
Synopsis
Group commands
Options
-h, --help        help for group
-q, --jq string   jq query string override

<!-- Page 186 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins  - Conﬁg jenkins
studio-cli jenkins group assign  - Assign group access
studio-cli jenkins group revoke  - Revoke group access
studio-cli jenkins group assign
Assign group access
Synopsis
Assign a group access
studio-cli jenkins group assign [flags]
Options
-g, --group string      group name.
-h, --help              help for assign
-n, --name string       Resource name
-r, --rolename string   Role (viewer, tester, editor, or lead) (default "viewer")
-w, --wrrn string       wrrn of resource.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 187 -->

## See Also
studio-cli jenkins group  - Group commands
studio-cli jenkins group revoke
Revoke group access
Synopsis
Revoke a group access
studio-cli jenkins group revoke [flags]
Options
-g, --group string   group name.
-h, --help           help for revoke
-n, --name string    Resource name
-w, --wrrn string    wrrn of resource.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins group  - Group commands
studio-cli jenkins pipeline
pipeline commands
Synopsis
pipeline commands

<!-- Page 188 -->

Options
-h, --help        help for pipeline
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins  - Conﬁg jenkins
studio-cli jenkins pipeline build  - Build commands
studio-cli jenkins pipeline create  - Create a Jenkins item from a template with RBAC default
permissions
studio-cli jenkins pipeline delete  - Delete resource
studio-cli jenkins pipeline get  - Get resource
studio-cli jenkins pipeline search  - Search resource
studio-cli jenkins pipeline update  - Add a group with a predeﬁned role to a jenkins conﬁguration
item
studio-cli jenkins pipeline build
Build commands
Synopsis
Build commands
Options
-h, --help   help for build
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 189 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins pipeline  - pipeline commands
studio-cli jenkins pipeline build start  - Start build a Jenkins Job
studio-cli jenkins pipeline build status  - Build status of a Jenkins Job
studio-cli jenkins pipeline build tail  - Print console log of jenkins build
studio-cli jenkins pipeline build start
Start build a Jenkins Job
Synopsis
Start build a Jenkins Job
studio-cli jenkins pipeline build start [flags]
Options
-f, --file string          Provide Params in json/yaml format via a file
-h, --help                 help for start
-j, --job string           Specify jenkins job path e.g. /wrlinux/wrlinux-USP-build
-p, --params stringArray   Params (e.g. --param param1=value1)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins pipeline build  - Build commands

<!-- Page 190 -->

studio-cli jenkins pipeline build status
Build status of a Jenkins Job
Synopsis
Build status of a Jenkins Job
studio-cli jenkins pipeline build status [flags]
Options
-b, --build-id string   Specify jenkins build id
-h, --help              help for status
-j, --job string        Specify jenkins job path e.g. /wrlinux/wrlinux-USP-build
--quiet             When waiting for the jenkins builds to complete do not emit
status
-s, --sleep int         Sleep poll interval seconds until jenkins build is finished, must
be greater than 0 (default 10)
-w, --wait              Wait for the jenkins build to complete
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins pipeline build  - Build commands
studio-cli jenkins pipeline build tail
Print console log of jenkins build
Synopsis
Print console log of jenkins build

<!-- Page 191 -->

studio-cli jenkins pipeline build tail [flags]
Options
-h, --help         help for tail
-j, --job string   Tail by job name e.g. /wrlinux/wrlinux-USP-build/1
-n, --nowait       Do not wait for log to complete
-s, --sleep int    Sleep poll interval in seconds (default: 2) (default 2)
-u, --url string   Specify fully qualified jenkins URL
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins pipeline build  - Build commands
studio-cli jenkins pipeline create
Create a Jenkins item from a template with RBAC default permissions
Synopsis
Create a Jenkins item such as a Jenkins project.Each created item uses a pre-deﬁned permissions
template applied.
studio-cli jenkins pipeline create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",

<!-- Page 192 -->

"tags": "{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override

<!-- Page 193 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins pipeline  - pipeline commands
studio-cli jenkins pipeline delete
Delete resource
Synopsis
Delete resource
studio-cli jenkins pipeline delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins pipeline  - pipeline commands
studio-cli jenkins pipeline get
Get resource

<!-- Page 194 -->

Synopsis
Get resource
studio-cli jenkins pipeline get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins pipeline  - pipeline commands
studio-cli jenkins pipeline search
Search resource
Synopsis
Search resource
studio-cli jenkins pipeline search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys

<!-- Page 195 -->

-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "folder")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 196 -->

-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins pipeline  - pipeline commands
studio-cli jenkins pipeline update
Add a group with a predeﬁned role to a jenkins conﬁguration item
Synopsis
Add a group with a predeﬁned role to a jenkins conﬁguration item. Y ou can choose from the roles of :
lead, dev eloper, tester, view er
studio-cli jenkins pipeline update [flags]
Options
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change
-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn

<!-- Page 197 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins pipeline  - pipeline commands
studio-cli jenkins token
Token commands
Synopsis
Token commands
Options
-h, --help   help for token
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins  - Conﬁg jenkins
studio-cli jenkins token add  - Add token to jenkins
studio-cli jenkins token list  - List jenkins token
studio-cli jenkins token remov e - Remov e token from jenkins

<!-- Page 198 -->

studio-cli jenkins token add
Add token to jenkins
Synopsis
Add token to jenkins
studio-cli jenkins token add [flags]
Options
-h, --help          help for add
-n, --name string   Name of token
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins token  - Token commands
studio-cli jenkins token list
List jenkins token
Synopsis
List jenkins token
studio-cli jenkins token list [flags]
Options
-h, --help   help for list

<!-- Page 199 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins token  - Token commands
studio-cli jenkins token remove
Remov e token from jenkins
Synopsis
Remov e token from jenkins
studio-cli jenkins token remove [flags]
Options
-h, --help        help for remove
-i, --id string    Access token key id to remove from jenkins profile (id from tokenlist)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli jenkins token  - Token commands

<!-- Page 200 -->

studio-cli lxbs
Linux Build System commands
Synopsis
Linux Build System Commands
Options
-h, --help        help for lxbs
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli lxbs build  - Build commands
studio-cli lxbs conﬁguration  - Conﬁguration commands
studio-cli lxbs dashboard  - Show my projects
studio-cli lxbs group  - Group commands
studio-cli lxbs project  - Project commands
studio-cli lxbs build
Build commands
Synopsis
Build commands
Options
-h, --help   help for build

<!-- Page 201 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs  - Linux Build System commands
studio-cli lxbs build cancel  - Cancel a build for a project.
studio-cli lxbs build get  - Get details of a speciﬁc build.
studio-cli lxbs build list-activ e - Search through the activ e builds
studio-cli lxbs build list-completed  - Search through the completed builds
studio-cli lxbs build start  - Start the building of the project.
studio-cli lxbs build cancel
Cancel a build for a project.
Synopsis
Cancel a build for a project.
studio-cli lxbs build cancel [flags]
Options
-b, --buildid string   Cancel by build id
-h, --help             help for cancel
-i, --id string        Cancel all builds for project id
-n, --name string      Cancel all builds for project name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 202 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs build  - Build commands
studio-cli lxbs build get
Get details of a speciﬁc build.
Synopsis
Get details of a speciﬁc build.
studio-cli lxbs build get [flags]
Options
-h, --help        help for get
-i, --id string   Uuid of the build task
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs build  - Build commands
studio-cli lxbs build list-active
Search through the activ e builds

<!-- Page 203 -->

Synopsis
Search through the activ e builds
studio-cli lxbs build list-active [flags]
Options
-b, --builduuid strings   Check active status on specific Build UUID's or wait on them
-h, --help                help for list-active
-i, --id string           List builds by this project id
-l, --limit int           Limit project list to X entries per page (0 for all) (default
10)
-n, --name string         List builds by this project name
--nofilter            Do not use the task filter for logging
-p, --page int            Page number of listing when using a limit (default 1)
--quiet               When waiting for the build(s) to complete do not emit status
-s, --sleep int           Sleep poll interval in seconds" default:"5" (default 5)
-w, --wait                Wait for the build(s) to complete
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs build  - Build commands
studio-cli lxbs build list-completed
Search through the completed builds
Synopsis
Search through the completed builds
studio-cli lxbs build list-completed [flags]

<!-- Page 204 -->

Options
-h, --help          help for list-completed
-i, --id string     List builds by this project id
-l, --limit int     Limit project list to X entries per page (use 0 for all) (default:
20) (default 10)
-n, --name string   List builds by this project name
-p, --page int      Page number of listing when using a limit (default 1)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs build  - Build commands
studio-cli lxbs build start
Start the building of the project.
Synopsis
Start the building of the project.
studio-cli lxbs build start [flags]
Options
-a, --all                    Build for all specified machines
--artifact-path string   Override the artifact path (used by plm)
-h, --help                   help for start
-i, --id string              Project id
-m, --machine strings        Select a specific machine or all for multi-machine
configurations
-n, --name string            Project name
--nofilter               Do not use the task filter for logging
--quiet                  When waiting for the build(s) to complete do not emit status
-s, --sleep int              Sleep poll interval in seconds" default:"5" (default 5)
-w, --wait                   Wait for the build(s) to complete

<!-- Page 205 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs build  - Build commands
studio-cli lxbs configuration
Conﬁguration commands
Synopsis
Conﬁguration commands
Options
-h, --help   help for configuration
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 206 -->

## See Also
studio-cli lxbs  - Linux Build System commands
studio-cli lxbs conﬁguration list-branch  - Return list of branches and rcpls
studio-cli lxbs conﬁguration setup-access-conﬁg  - Conﬁgure the access conﬁguration for XBS with
necessary resources
studio-cli lxbs conﬁguration setup-existing-access-conﬁg  - Conﬁgure and existing access
conﬁguration for LXBS with existing resources
studio-cli lxbs conﬁguration teardown-access-conﬁg  - Teardown the access conﬁguration and
dependent resources
studio-cli lxbs configuration list-branch
Return list of branches and rcpls
Synopsis
Return list of branches and rcpls
studio-cli lxbs configuration list-branch [flags]
Options
-h, --help          help for list-branch
-n, --name string   Access config name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs conﬁguration  - Conﬁguration commands

<!-- Page 207 -->

studio-cli lxbs configuration setup-access-config
Conﬁgure the access conﬁguration for XBS with necessary resources
Synopsis
Conﬁgure the access conﬁguration for XBS with necessary resources
studio-cli lxbs configuration setup-access-config [flags]
Options
-a, --artifacts-bucket string   Artifacts bucket, needs to be passed in the format of
workspace-<bucket-name>
-g, --gitlab-group string       Gitlab group
-h, --help                      help for setup-access-config
-n, --name string               Access config name
-r, --releases string           Releases
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs conﬁguration  - Conﬁguration commands
studio-cli lxbs configuration setup-existing-access-config
Conﬁgure and existing access conﬁguration for LXBS with existing resources
Synopsis
Conﬁgure and existing access conﬁguration for LXBS with existing resources, it creates the access
conﬁguration if it doesn’t exist
studio-cli lxbs configuration setup-existing-access-config [flags]

<!-- Page 208 -->

Options
-a, --artifacts-bucket string   Artifacts bucket (comma separated, each item should be in
the format of workspace-<bucket-name>)
-g, --gitlab-group string       Gitlab group (comma separated)
-h, --help                      help for setup-existing-access-config
-n, --name string               Access config name, if it doesn't exist it will be
created
-r, --releases string           Releases (comma separated)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs conﬁguration  - Conﬁguration commands
studio-cli lxbs configuration teardown-access-config
Teardown the access conﬁguration and dependent resources
Synopsis
Teardown the access conﬁguration and dependent resources
studio-cli lxbs configuration teardown-access-config [flags]
Options
-a, --artifacts-bucket string   Artifacts bucket (comma separated, each item should be in
the format of workspace-<bucket-name>)
-g, --gitlab-group string       Gitlab group (comma separated)
-h, --help                      help for teardown-access-config
-n, --name string               Access config name
-r, --releases string           Releases

<!-- Page 209 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs conﬁguration  - Conﬁguration commands
studio-cli lxbs dashboard
Show my projects
Synopsis
Show my projects
studio-cli lxbs dashboard [flags]
Options
-h, --help          help for dashboard
-l, --limit int     Limit project list to X entries per page (default: 0 for all)
(default 10)
-n, --name string   List projects matching partial name
-p, --page int      Page number of listing when using a limit (default 1)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 210 -->

## See Also
studio-cli lxbs  - Linux Build System commands
studio-cli lxbs group
Group commands
Synopsis
Group commands
Options
-h, --help   help for group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs  - Linux Build System commands
studio-cli lxbs group assign  - Assign group access
studio-cli lxbs group revoke  - Revoke group access
studio-cli lxbs group assign
Assign group access
Synopsis
Assign a new group access.
studio-cli lxbs group assign [flags]

<!-- Page 211 -->

Options
-g, --group string      group name
-h, --help              help for assign
-i, --id string         Project id
-n, --name string       Name of project
-r, --rolename string   Role (viewer, tester, editor, or lead) (default "viewer")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs group  - Group commands
studio-cli lxbs group revoke
Revoke group access
Synopsis
Revoke a group access.
studio-cli lxbs group revoke [flags]
Options
-g, --group string   group name
-h, --help           help for revoke
-i, --id string      Project id
-n, --name string    Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string

<!-- Page 212 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs group  - Group commands
studio-cli lxbs project
Project commands
Synopsis
Project commands
Options
-h, --help   help for project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 213 -->

## See Also
studio-cli lxbs  - Linux Build System commands
studio-cli lxbs project archiv e - Mov e project to Archiv e list
studio-cli lxbs project assign-access-conﬁg  - Assign a project to an access conﬁg
studio-cli lxbs project clone  - Clone a project.
studio-cli lxbs project create  - Create a new project
studio-cli lxbs project create-bulk  - Bulk create projects from Y AML ﬁle
studio-cli lxbs project export  - Export a project to a ﬁle
studio-cli lxbs project get  - Get project information.
studio-cli lxbs project import  - Import a project from a ﬁle
studio-cli lxbs project lay er - Lay er commands
studio-cli lxbs project list  - Search through projects
studio-cli lxbs project local-conf  - Local-conf commands
studio-cli lxbs project remov e - Permanently remov e project (must be owner or admin)
studio-cli lxbs project repo  - Get all the details of a project from the git repository.
studio-cli lxbs project restore  - Restore archiv ed project
studio-cli lxbs project update  - Update a project using an input ﬁle
studio-cli lxbs project update-release  - Update the release of that existing project.
studio-cli lxbs project archive
Mov e project to Archiv e list
Synopsis
Mov e project to Archiv e list
studio-cli lxbs project archive [flags]
Options
-h, --help          help for archive
-i, --id string     Project id
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 214 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project assign-access-config
Assign a project to an access conﬁg
Synopsis
Assign a project to an access conﬁg
studio-cli lxbs project assign-access-config [flags]
Options
-a, --access-config string   access-config name
-h, --help                   help for assign-access-config
-n, --name string            project name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands

<!-- Page 215 -->

studio-cli lxbs project clone
Clone a project.
Synopsis
Clone a project.
studio-cli lxbs project clone [flags]
Options
-a, --access string               Sharing access level, LEAD|EDITOR|TESTER|VIEWER
(default "LEAD")
-f, --access-config-name string   Access config name
-c, --cname string                The existing project name
-p, --gitlab-group-name string    Gitlab Group Name
-g, --groupname string            Group name
-h, --help                        help for clone
-i, --id string                   The existing project id
-n, --name string                 Name of the new project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project create
Create a new project
Synopsis
Create a new project
studio-cli lxbs project create [flags]

<!-- Page 216 -->

Options
-a, --access string               sharing access level, LEAD|EDITOR|TESTER|VIEWER
(default "LEAD")
-f, --access-config-name string   Access config name
-b, --branch string               Branch name for project
-d, --desc string                 Optional description for project
-p, --gitlab-group-name string    Gitlab Group Name
-g, --groupname string            group name
-h, --help                        help for create
-i, --image string                Optional image name
-m, --machine string              Optional machine to configure for project
-n, --name string                 Name of project
-r, --rcpl string                 RCPL patch number or HEAD
--skip-recommended            Skip adding the recommended layers
-t, --template                    Create Project as template
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project create-bulk
Bulk create projects from Y AML ﬁle
Synopsis
Bulk create projects from Y AML ﬁle
studio-cli lxbs project create-bulk [flags]
Options
-a, --access string               sharing access level, LEAD|EDITOR|TESTER|VIEWER
(default "LEAD")
-f, --access-config-name string   Access config name
-b, --branch string               Branch name for project

<!-- Page 217 -->

-p, --gitlab-group-name string    Gitlab group name
-g, --group-name string           group name
-h, --help                        help for create-bulk
-r, --rcpl string                 RCPL patch number or HEAD
--skip-recommended            Skip adding the recommended layers
-y, --yamlfile string             YAML file contains project metadata
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project export
Export a project to a ﬁle
Synopsis
Export a project to a ﬁle
studio-cli lxbs project export [flags]
Options
-f, --file string   Output to a file
-h, --help          help for export
-i, --id string     Project id
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 218 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project get
Get project information.
Synopsis
Get project information.
studio-cli lxbs project get [flags]
Options
-h, --help          help for get
-i, --id string     Project id
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project import
Import a project from a ﬁle

<!-- Page 219 -->

Synopsis
Import a project from a ﬁle
studio-cli lxbs project import [flags]
Options
-c, --access-config-name string   Access config name
-e, --desc string                 Optional description for project
-f, --file string                 Get input from a json/yaml file
-p, --gitlab-group-name string    Gitlab Group Name
-g, --groupname string            group name
-h, --help                        help for import
-n, --name string                 New name for imported project
--skip-recommended            Skip adding the recommended layers
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project layer
Layer commands
Synopsis
Project Lay er commands.
Options
-h, --help   help for layer

<!-- Page 220 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project lay er admin  - Admin functions for WR Linux project defaults and content
loading
studio-cli lxbs project lay er list  - Print the project default v alues for lay ers and local.conf
studio-cli lxbs project lay er update  - Process and update a project’s recommended lay ers
studio-cli lxbs project layer admin
Admin functions for WR Linux project defaults and content loading
Synopsis
Admin functions for WR Linux project defaults and content loading.
studio-cli lxbs project layer admin [flags]
Options
-b, --branch string   Use a different branch other than master for the git tree (default:
master)
-h, --help            help for admin
-p, --progress        Show the progress of the layer reloading
-t, --token string    Use an alternate admin token
-u, --url string      git URL to use for the load process
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 221 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project lay er - Lay er commands
studio-cli lxbs project layer list
Print the project default v alues for lay ers and local.conf
Synopsis
Print the project default v alues for lay ers and local.conf
studio-cli lxbs project layer list [flags]
Options
-h, --help          help for list
-i, --id string     Project id
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project lay er - Lay er commands
studio-cli lxbs project layer update
Process and update a project’s recommended lay ers

<!-- Page 222 -->

Synopsis
Process and update a project’s recommended lay ers
studio-cli lxbs project layer update [flags]
Options
-h, --help           help for update
-i, --id string      Project id
-l, --layer string   Add an additional layer
-n, --name string    Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project lay er - Lay er commands
studio-cli lxbs project list
Search through projects
Synopsis
Search through projects
studio-cli lxbs project list [flags]
Options
--archive            Archive project list
-h, --help               help for list
-l, --limit int          Limit project list to X entries per page (0 for all) (default
10)
-n, --name string        List projects matching partial name

<!-- Page 223 -->

-p, --page int           Page number of listing when using a limit (default 1)
--show-my-projects   Show my projects
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project local-conf
Local-conf commands
Synopsis
Local-conf commands.
Options
-h, --help   help for local-conf
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 224 -->

## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project local-conf get  - Get project’s local.conf with ${EDITOR}
studio-cli lxbs project local-conf put  - Put project’s local.conf with ${EDITOR} to serv er
studio-cli lxbs project local-conf get
Get project’s local.conf with ${EDITOR}
Synopsis
Get project’s local.conf with ${EDITOR}
studio-cli lxbs project local-conf get [flags]
Options
-h, --help                help for get
-i, --id string           Project id
-f, --local-file string   saved local-conf file name
-n, --name string         Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project local-conf  - Local-conf commands
studio-cli lxbs project local-conf put
Put project’s local.conf with ${EDITOR} to serv er

<!-- Page 225 -->

Synopsis
Put project’s local.conf with ${EDITOR} to serv er
studio-cli lxbs project local-conf put [flags]
Options
-h, --help                help for put
-i, --id string           Project id
-f, --local-file string   Replace the project's local.conf with the specified file
-n, --name string         Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project local-conf  - Local-conf commands
studio-cli lxbs project remove
Permanently remov e project (must be owner or admin)
Synopsis
Permanently remov e project (must be owner or admin)
studio-cli lxbs project remove [flags]
Options
-h, --help          help for remove
-i, --id string     Project id
-n, --name string   Name of project

<!-- Page 226 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project repo
Get all the details of a project from the git repository.
Synopsis
Get all the details of a project from the git repository.
studio-cli lxbs project repo [flags]
Options
-h, --help          help for repo
-i, --id string     Id of the new project
-n, --name string   Name of the new project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands

<!-- Page 227 -->

studio-cli lxbs project restore
Restore archiv ed project
Synopsis
Restore archiv ed project
studio-cli lxbs project restore [flags]
Options
-h, --help          help for restore
-i, --id string     Project id
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project update
Update a project using an input ﬁle
Synopsis
Update a project using an input ﬁle
studio-cli lxbs project update [flags]
Options
--editor strings   group name
-f, --file string      Get input from a json/yaml file
-h, --help             help for update
-i, --id string        Update a different project than the input file

<!-- Page 228 -->

--lead strings     group name
-n, --name string      Update a different project than the input file
--tester strings   group name
--viewer strings   group name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli lxbs project update-release
Update the release of that existing project.
Synopsis
Update the release of that existing project.
studio-cli lxbs project update-release [flags]
Options
-b, --branch string   New branch name for project
-h, --help            help for update-release
-i, --id string       Id of the new project
-n, --name string     Name of the new project
-r, --rcpl string     New RCPL patch number or HEAD
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 229 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli lxbs project  - Project commands
studio-cli ota
OTA commands
Synopsis
Over-The-Air commands.
Options
-h, --help   help for ota
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli ota certiﬁcates  - OT A certiﬁcates command
studio-cli ota devicegroups  - OT A device groups command
studio-cli ota devices  - OT A devices command
studio-cli ota groups  - OT A groups command
studio-cli ota packages  - OT A packages command
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota releases  - OT A releases command

<!-- Page 230 -->

studio-cli ota certificates
OTA certiﬁcates command
Synopsis
OTA certiﬁcates command.
Options
-h, --help   help for certificates
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota  - OT A commands
studio-cli ota certiﬁcates generate  - Generate certiﬁcate command
studio-cli ota certificates generate
Generate certiﬁcate command
Synopsis
Generate certiﬁcate command.
studio-cli ota certificates generate [flags]
Options
-d, --device-group string    device group
-h, --help                   help for generate
-o, --output-folder string   output folder

<!-- Page 231 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota certiﬁcates  - OT A certiﬁcates command
studio-cli ota devicegroups
OTA device groups command
Synopsis
OTA device groups command.
Options
-h, --help   help for devicegroups
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota  - OT A commands
studio-cli ota devicegroups create  - Create device group command

<!-- Page 232 -->

studio-cli ota devicegroups create
Create device group command
Synopsis
Create device group command.
studio-cli ota devicegroups create [flags]
Options
-d, --description string   description
-g, --devicegroup string   device group name
-h, --help                 help for create
-u, --usergroup string     user group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota devicegroups  - OT A device groups command
studio-cli ota devices
OTA devices command
Synopsis
OTA devices command.
Options
-h, --help   help for devices

<!-- Page 233 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota  - OT A commands
studio-cli ota devices create  - Create devices command
studio-cli ota devices get  - Get device by id command
studio-cli ota devices getall  - Get all devices command
studio-cli ota devices create
Create devices command
Synopsis
Create devices command.
studio-cli ota devices create [flags]
Options
-t, --devicetype string   device type
-h, --help                help for create
-i, --id string           device id
-l, --location string     device location
-m, --model string        device model
-p, --payload string      json file directory with the device information to be created
-y, --year string         device year
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 234 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota devices  - OT A devices command
studio-cli ota devices get
Get device by id command
Synopsis
Get device by id command
studio-cli ota devices get [flags]
Options
-g, --devicegroup string   device group deprecated. Please use devicetype instead
-t, --devicetype string    device type
-h, --help                 help for get
-i, --id string            device id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota devices  - OT A devices command
studio-cli ota devices getall
Get all devices command

<!-- Page 235 -->

Synopsis
Get all devices command
studio-cli ota devices getall [flags]
Options
-t, --devicetype string   device type
-h, --help                help for getall
-n, --pagenumber int32    Page number
-s, --pagesize int32      Page size (default 10)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota devices  - OT A devices command
studio-cli ota groups
OTA groups command
Synopsis
OTA groups command.
Options
-h, --help   help for groups
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode

<!-- Page 236 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota  - OT A commands
studio-cli ota groups getall  - Get all groups command
studio-cli ota groups getall
Get all groups command
Synopsis
Get all groups command
studio-cli ota groups getall [flags]
Options
-g, --devicegroup string   device group deprecated. Please use devicetype instead
-t, --devicetype string    device type
-h, --help                 help for getall
-l, --location string      filter by location
-o, --model string         filter by model
-y, --year string          filter by year
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota groups  - OT A groups command

<!-- Page 237 -->

studio-cli ota packages
OTA packages command
Synopsis
OTA packages command.
Options
-h, --help   help for packages
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota  - OT A commands
studio-cli ota packages create  - Create package command
studio-cli ota packages delete  - Delete package by id command
studio-cli ota packages get  - Get package command
studio-cli ota packages getall  - Get All packages command
studio-cli ota packages update  - Update package command
studio-cli ota packages create
Create package command
Synopsis
Create package command
Considerations: 1-If a ﬂag(except pay load) has already been sent and this exists inside the pay load ﬁle
it will be ov erwrite the v alue of that property. 2-P ayload ﬂag required due to ota cli doesn’t support
sending arrays v alues in ﬂag.
studio-cli ota packages create [flags]

<!-- Page 238 -->

Options
-g, --accessgroup string   access group
-d, --description string   package description
-e, --devicetype string    device type
-h, --help                 help for create
-p, --payload string       json file directory with the package information to be
created. Required to send payloads property
-s, --state string         package state
-t, --title string         package title
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota packages  - OT A packages command
studio-cli ota packages delete
Delete package by id command
Synopsis
Delete package by id command.
studio-cli ota packages delete [flags]
Options
-h, --help           help for delete
-t, --title string   title
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode

<!-- Page 239 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota packages  - OT A packages command
studio-cli ota packages get
Get package command
Synopsis
Get package command
studio-cli ota packages get [flags]
Options
-h, --help           help for get
-t, --title string   title
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota packages  - OT A packages command
studio-cli ota packages getall
Get All packages command
Synopsis
Get All packages command.

<!-- Page 240 -->

studio-cli ota packages getall [flags]
Options
-h, --help                help for getall
-n, --pagenumber string   page number (default "1")
-s, --pagesize string     page size (default "10")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota packages  - OT A packages command
studio-cli ota packages update
Update package command
Synopsis
Update package command.
Considerations: 1-If a ﬂag(except pay load) has already been sent and this exists inside the pay load ﬁle
it will be ov erwrite the v alue of that property. 2-P ayload ﬂag required to update pay loads property due
to ota cli doesn’t support sending arrays v alues in ﬂag.
studio-cli ota packages update [flags]
Options
-d, --description string   package description
-h, --help                 help for update
-p, --payload string       json file directory with the package information to be
updated. Required to update payloads property
-s, --state string         package state
-t, --title string         package title

<!-- Page 241 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota packages  - OT A packages command
studio-cli ota payloads
OTA pay loads command
Synopsis
OTA pay loads command.
Options
-h, --help   help for payloads
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 242 -->

## See Also
studio-cli ota  - OT A commands
studio-cli ota pay loads create  - Creates a new pay load
studio-cli ota pay loads create-automated  - Creates a new multipart pay load with a single step
studio-cli ota pay loads createmultipart  - Creates a new multipart pay load
studio-cli ota pay loads delete  - Delete a pay load
studio-cli ota pay loads get  - Get pay load by Id
studio-cli ota pay loads getall  - Get all pay loads
studio-cli ota pay loads getbackground  - Get background job progress
studio-cli ota pay loads multipartmerge  - Merges multiple multipart pay loads
studio-cli ota pay loads sign  - Signs a pay load ﬁle
studio-cli ota pay loads split  - Splits the signed pay load ﬁle
studio-cli ota pay loads uploadchunk  - Uploads a pay load chunk
studio-cli ota pay loads uploadcomplete  - Uploads a complete multipart pay load
studio-cli ota payloads create
Creates a new pay load
Synopsis
Create payload command
This command creates a new payload file based on the provided input.
studio-cli ota payloads create [flags]
Options
-d, --devicegroup string   device group for payload creation
-f, --filepath string      input file for payload creation
-h, --help                 help for create
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode

<!-- Page 243 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads create-automated
Creates a new multipart pay load with a single step
Synopsis
Create multipart payload command
This command is an automation of all the step of multipart payload creation
studio-cli ota payloads create-automated [flags]
Options
-h, --help             help for create-automated
-p, --payload string   JSON payload file path
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads createmultipart
Creates a new multipart pay load

<!-- Page 244 -->

Synopsis
Create multipart payload command
This command creates a new multipart payload file based on the provided input.
studio-cli ota payloads createmultipart [flags]
Options
-d, --devicegroup string   device group for multipart payload creation
-f, --filename string      input file for multipart payload creation
-h, --help                 help for createmultipart
-s, --sha256 string        SHA256 hash for multipart payload creation
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads delete
Delete a pay load
Synopsis
Delete a payload command
This command deletes a payload based on the specified device group and payload ID.
studio-cli ota payloads delete [flags]
Options
--devicegroup string   Device group
-h, --help                 help for delete

<!-- Page 245 -->

--id string            Payload Id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads get
Get pay load by Id
Synopsis
Get payload by Id command
This command retrieves a payload by its ID and displays it.
studio-cli ota payloads get [flags]
Options
--devicegroup string   Device Group
-h, --help                 help for get
--id string            Payload Id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 246 -->

## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads getall
Get all pay loads
Synopsis
Get all payloads command
This command retrieves all payloads and displays them.
studio-cli ota payloads getall [flags]
Options
--devicegroup string   Device group
-h, --help                 help for getall
--pagenumber string    Page number (default "1")
--pagesize string      Page size (default "10")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads getbackground
Get background job progress

<!-- Page 247 -->

Synopsis
Get background upload command
This command retrieves a payload upload progress by its job id and displays it.
studio-cli ota payloads getbackground [flags]
Options
-h, --help        help for getbackground
--id string   Background job id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads multipartmerge
Merges multiple multipart pay loads
Synopsis
Multipart merge command
This command merges multiple multipart payloads based on the provided input.
studio-cli ota payloads multipartmerge [flags]
Options
-f, --filepath string   JSON payloads file path
-h, --help              help for multipartmerge

<!-- Page 248 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads sign
Signs a pay load ﬁle
Synopsis
Signs a payload file
The payload file will be signed using the provided private key and certificate, and the
signed payload will be saved in the same directory as the payload ZIP file.
studio-cli ota payloads sign [flags]
Options
-h, --help                  help for sign
-f, --payload-file string   .zip payload file
-s, --sign-cert string      signing cert file
-p, --sign-private string   signing private file
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 249 -->

## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads split
Splits the signed pay load ﬁle
Synopsis
Split payload command
The payload file will be split into n number of chunks, these chunks will be stored in the
output path. A json will also be created with the sha256 of each of the chunks created.
studio-cli ota payloads split [flags]
Options
-f, --filepath string    payload file to split
-h, --help               help for split
-o, --outputdir string   directory to save the chunks
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads uploadchunk
Uploads a pay load chunk

<!-- Page 250 -->

Synopsis
Upload payload chunk command
This command uploads a payload chunk based on the provided input. The chunk will be
associated with the multipart payload specified by the filename, device group, and SHA256
hash.
studio-cli ota payloads uploadchunk [flags]
Options
-i, --chunkindex string    index of the payload chunk
-d, --devicegroup string   device group for multipart payload
-f, --filepath string      payload chunk filepath
-h, --help                 help for uploadchunk
-s, --sha256 string        SHA256 hash for complete payload file
-t, --uploadtoken string   upload token for chunk upload
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota payloads uploadcomplete
Uploads a complete multipart pay load
Synopsis
Upload complete multipart payload command
This command uploads a complete multipart payload based on the provided input.
studio-cli ota payloads uploadcomplete [flags]

<!-- Page 251 -->

Options
-p, --filepath string   JSON payload file path
-h, --help              help for uploadcomplete
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota pay loads  - OT A pay loads command
studio-cli ota releases
OTA releases command
Synopsis
OTA releases command.
Options
-h, --help   help for releases
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 252 -->

## See Also
studio-cli ota  - OT A commands
studio-cli ota releases create  - Create release command
studio-cli ota releases delete  - Delete release command
studio-cli ota releases get  - Get release
studio-cli ota releases getall  - Get all releases command
studio-cli ota releases gettargets  - Get targets of the group selected in the release
studio-cli ota releases selecttargets  - Select targets to release command
studio-cli ota releases setstate  - Set state for release command
studio-cli ota releases update  - Update release command
studio-cli ota releases create
Create release command
Synopsis
Create release command.
Considerations: 1-If a ﬂag(except pay load) has already been sent and this exists inside the pay load ﬁle
it will be ov erwrite the v alue of that property. 2-P ayload ﬂag required due to ota cli doesn’t support
sending arrays v alues in ﬂag.
studio-cli ota releases create [flags]
Options
-g, --accessgroup string      access group
-d, --description string      release description
-v, --devicetype string       device type
-e, --enddate string          release end date
-h, --help                    help for create
-n, --name string             release name
-o, --otaenginegroup string   target's ota engine group to apply the release
-p, --payload string          json file directory with release information to be created.
Required to send packages property
-t, --selecttargets string    select targets
-s, --startdate string        release start date
-a, --state string            state
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 253 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota releases  - OT A releases command
studio-cli ota releases delete
Delete release command
Synopsis
Delete release command
studio-cli ota releases delete [flags]
Options
-h, --help          help for delete
-n, --name string   release name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota releases  - OT A releases command
studio-cli ota releases get
Get release

<!-- Page 254 -->

Synopsis
Get release
studio-cli ota releases get [flags]
Options
-h, --help          help for get
-n, --name string   release name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota releases  - OT A releases command
studio-cli ota releases getall
Get all releases command
Synopsis
Get all releases command.
studio-cli ota releases getall [flags]
Options
-h, --help                help for getall
-n, --pagenumber string   Page number when the result has multiple pages (default "1")
-s, --pagesize string     Page size of releases per page (use 0 for all) (default "10")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses

<!-- Page 255 -->

--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota releases  - OT A releases command
studio-cli ota releases gettargets
Get targets of the group selected in the release
Synopsis
Get targets of the group selected in the release
studio-cli ota releases gettargets [flags]
Options
-h, --help                help for gettargets
-n, --name string         release name
-p, --pagenumber string   page number (default "1")
-s, --pagesize string     page size (default "10")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota releases  - OT A releases command
studio-cli ota releases selecttargets
Select targets to release command

<!-- Page 256 -->

Synopsis
Select targets to release command.
studio-cli ota releases selecttargets [flags]
Options
-h, --help             help for selecttargets
-n, --name string      release name
-p, --payload string   json file directory with the list of targets
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota releases  - OT A releases command
studio-cli ota releases setstate
Set state for release command
Synopsis
Set state for release command
studio-cli ota releases setstate [flags]
Options
-h, --help           help for setstate
-n, --name string    release name
-s, --state string   release state to set
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 257 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ota releases  - OT A releases command
studio-cli ota releases update
Update release command
Synopsis
Update release command.
Considerations: 1-If a ﬂag(except pay load) has already been sent and this exists inside the pay load it
will be ov erwrite the v alue of that property. 2-P ayload ﬂag required due to ota cli doesn’t support
sending arrays v alues in ﬂag.
studio-cli ota releases update [flags]
Options
-d, --description string      release description
-e, --enddate string          release end date
-h, --help                    help for update
-n, --name string             release name
-o, --otaenginegroup string   ota engine group
-p, --payload string          json file directory with the release information to be
updated. Use it if you want to update packages property
-t, --selecttargets string    select targets
-s, --startdate string        release start date
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 258 -->

## See Also
studio-cli ota releases  - OT A releases command
studio-cli platformhealth
Platform Health commands
Synopsis
Platform Health commands
Options
-h, --help        help for platformhealth
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli platformhealth http  - Submit calls directly to the REST API
studio-cli platformhealth http
Submit calls directly to the REST API
Synopsis
The http subcommand works like a curl command where you specify the type of request and data via
the –data or –bodyﬁle argument if it is a POST or PUT call.
If the ﬁrst character of the -d/–data argument is an @ character, what follows will be used as a ﬁlename.
studio-cli platformhealth http [flags]

<!-- Page 259 -->

Options
-b, --bodyfile string   Data from file for a POST or PUT type call
-d, --data string       Data for a POST or PUT type call
-e, --endpoint string   end point
-h, --help              help for http
-s, --show              how to show response, 'raw' shows raw response body, 'format'
shows self-defined json response with status code, [raw|format]
-t, --type              http request type, [DELETE|GET|HEAD|POST|PUT]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli platformhealth  - Platform Health commands
studio-cli plm
Pipeline Manager
Synopsis
Pipeline Manager
Options
-h, --help   help for plm
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 260 -->

## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli plm access-conﬁg  - Pipeline Access Conﬁg commands
studio-cli plm group  - Pipeline group commands
studio-cli plm http  - Submit calls directly to the REST API
studio-cli plm pipeline  - Pipeline commands
studio-cli plm resource  - Pipeline resource commands
studio-cli plm run  - Pipeline run commands
studio-cli plm secret  - Pipeline secret commands
studio-cli plm task  - Pipeline task commands
studio-cli plm trigger  - Pipeline trigger commands
studio-cli plm access-config
Pipeline Access Conﬁg commands
Synopsis
Pipeline Access Conﬁg commands.
Options
-h, --help        help for access-config
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 261 -->

## See Also
studio-cli plm  - Pipeline Manager
studio-cli plm access-conﬁg create  - Create Access Conﬁg
studio-cli plm access-conﬁg delete  - Delete Access Conﬁg
studio-cli plm access-conﬁg get  - Display the y aml/json for an Access Conﬁg
studio-cli plm access-conﬁg list  - List all the Access Conﬁg deﬁnitions
studio-cli plm access-conﬁg pipeline  - Pipeline group
studio-cli plm access-conﬁg user  - User command group
studio-cli plm access-config create
Create Access Conﬁg
Synopsis
Create a Pipeline Access Conﬁguration (Access Conﬁg) with the giv en name. Optionally provide the
username/password of an existing (preferably robot) account to be associated with this Access Conﬁg.
In the usual case that no credentials are provided, a new bot account will be created.
studio-cli plm access-config create [flags]
Options
-s, --create-ssh        Enable ssh-key creation [default value: true] (>= version 2403)
(default true)
-g, --group string      Provide group name or id (>= version 2312)
-h, --help              help for create
-n, --name string       Name of Access Config
-p, --password string   Password of Access User
-u, --username string   Username of Access User
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 262 -->

## See Also
studio-cli plm access-conﬁg  - Pipeline Access Conﬁg commands
studio-cli plm access-config delete
Delete Access Conﬁg
Synopsis
Delete Access Conﬁg
studio-cli plm access-config delete [flags]
Options
-h, --help          help for delete
-n, --name string   Access Config Name or WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm access-conﬁg  - Pipeline Access Conﬁg commands
studio-cli plm access-config get
Display the y aml/json for an Access Conﬁg
Synopsis
Display the y aml/json for an Access Conﬁg speciﬁed by name.
studio-cli plm access-config get [flags]

<!-- Page 263 -->

Options
-d, --details       Show details of Access Config
-h, --help          help for get
-n, --name string   Access Config Name or WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm access-conﬁg  - Pipeline Access Conﬁg commands
studio-cli plm access-config list
List all the Access Conﬁg deﬁnitions
Synopsis
List all the av ailable Access Conﬁg deﬁnitions
studio-cli plm access-config list [flags]
Options
-h, --help              help for list
-l, --limit int         Limit number of Access Configs to return. Values equal to 0 will
return all access configs. (default 10)
-o, --offset int        Return results starting at this offset (default 1)
-u, --username string   Access Configs available for a username
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 264 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm access-conﬁg  - Pipeline Access Conﬁg commands
studio-cli plm access-config pipeline
Pipeline group
Synopsis
Pipeline group
Options
-h, --help   help for pipeline
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm access-conﬁg  - Pipeline Access Conﬁg commands
studio-cli plm access-conﬁg pipeline list  - Display list of pipelines created with an Access Conﬁg
studio-cli plm access-config pipeline list
Display list of pipelines created with an Access Conﬁg
Synopsis
Display y aml/json formatted list of pipelines that w ere created with the speciﬁed Access Conﬁg

<!-- Page 265 -->

studio-cli plm access-config pipeline list [flags]
Options
-h, --help          help for list
-n, --name string   Access Config Name or WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm access-conﬁg pipeline  - Pipeline group
studio-cli plm access-config user
User command group
Synopsis
User command group
Options
-h, --help   help for user
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 266 -->

## See Also
studio-cli plm access-conﬁg  - Pipeline Access Conﬁg commands
studio-cli plm access-conﬁg user assign  - Assign user ability to create a pipeline with this Access
Conﬁg
studio-cli plm access-conﬁg user list  - Display list of users who hav e permission to create pipelines
with Access Conﬁg. This requires uspAdmin role
studio-cli plm access-conﬁg user revoke  - Revoke user ability to create a pipeline with this Access
Conﬁg
studio-cli plm access-config user assign
Assign user ability to create a pipeline with this Access Conﬁg
Synopsis
Assign user ability to create a pipeline with this Access Conﬁg
studio-cli plm access-config user assign [flags]
Options
-h, --help          help for assign
-n, --name string   Access Config Name or WRRN
-u, --user string   Username
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm access-conﬁg user  - User command group

<!-- Page 267 -->

studio-cli plm access-config user list
Display list of users who hav e permission to create pipelines with Access Conﬁg. This requires
uspAdmin role
Synopsis
Display list of users who hav e permission to create pipelines with the speciﬁed Access Conﬁg. This
requires uspAdmin role
studio-cli plm access-config user list [flags]
Options
-h, --help          help for list
-n, --name string   Access Config Name or WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm access-conﬁg user  - User command group
studio-cli plm access-config user revoke
Revoke user ability to create a pipeline with this Access Conﬁg
Synopsis
Revoke user ability to create a pipeline with this Access Conﬁg
studio-cli plm access-config user revoke [flags]
Options
-h, --help          help for revoke
-n, --name string   Access Config Name or WRRN

<!-- Page 268 -->

-u, --user string   Username
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm access-conﬁg user  - User command group
studio-cli plm group
Pipeline group commands
Synopsis
Pipeline group commands.
Options
-a, --access-config string   name or Wind River Resource Name (WRRN) of Access Config
-h, --help                   help for group
-q, --jq string              jq query string
-p, --pipeline string        name or id of pipeline
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 269 -->

## See Also
studio-cli plm  - Pipeline Manager
studio-cli plm group assign  - Assign group of users access to pipeline or Access Conﬁg
studio-cli plm group join  - Add the bot account for a pipeline or Access Conﬁg to a group
studio-cli plm group leav e - Remov e the bot account for a pipeline or Access Conﬁg from a group
studio-cli plm group revoke  - Revoke group access to pipeline or Access Conﬁg
studio-cli plm group assign
Assign group of users access to pipeline or Access Conﬁg
Synopsis
Assign group of users access to pipeline or Access Conﬁg with speciﬁed role
studio-cli plm group assign [flags]
Options
-i, --group-id string     Group id
-g, --group-name string   Group name
-h, --help                help for assign
-r, --role-name string    Role (viewer, tester, editor, or lead) (default "viewer")
Options inherited from parent commands
-a, --access-config string   name or Wind River Resource Name (WRRN) of Access Config
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        name or id of pipeline
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm group  - Pipeline group commands

<!-- Page 270 -->

studio-cli plm group join
Add the bot account for a pipeline or Access Conﬁg to a group
Synopsis
Add the bot account for a pipeline or Access Conﬁg to the named group. Use this to assign a pipeline
(or all the pipelines using an AccessConﬁg) permission to access a resource that the group has access
to.
studio-cli plm group join [flags]
Options
-g, --group-name string   Group name
-h, --help                help for join
Options inherited from parent commands
-a, --access-config string   name or Wind River Resource Name (WRRN) of Access Config
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        name or id of pipeline
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm group  - Pipeline group commands
studio-cli plm group leave
Remov e the bot account for a pipeline or Access Conﬁg from a group
Synopsis
Remov e the bot account for a pipeline or Access Conﬁg from the named group. Use this to revoke a
pipeline’s (or all the pipelines using an AccessConﬁg) permission to access a resource that the group
has access to.
studio-cli plm group leave [flags]

<!-- Page 271 -->

Options
-g, --group-name string   Group name
-h, --help                help for leave
Options inherited from parent commands
-a, --access-config string   name or Wind River Resource Name (WRRN) of Access Config
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        name or id of pipeline
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm group  - Pipeline group commands
studio-cli plm group revoke
Revoke group access to pipeline or Access Conﬁg
Synopsis
Revoke group access to pipeline or Access Conﬁg
studio-cli plm group revoke [flags]
Options
-i, --group-id string     Group id
-g, --group-name string   Group name
-h, --help                help for revoke
-r, --role-name string    Role (viewer, tester, editor, or lead) (default "viewer")
Options inherited from parent commands
-a, --access-config string   name or Wind River Resource Name (WRRN) of Access Config
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string

<!-- Page 272 -->

--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        name or id of pipeline
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm group  - Pipeline group commands
studio-cli plm http
Submit calls directly to the REST API
Synopsis
The http subcommand works like a curl command where you specify the type of request and data via
the –data or –bodyﬁle argument if it is a POST or PUT call.
If the ﬁrst character of the -d/–data argument is an @ character, what follows will be used as a ﬁlename.
studio-cli plm http [flags]
Options
-b, --bodyfile string   Data from file for a POST or PUT type call
-d, --data string       Data for a POST or PUT type call
-e, --endpoint string   end point (e.g. /pipeline/list)
-h, --help              help for http
-t, --type              http request type, [DELETE|GET|HEAD|POST|PUT]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm  - Pipeline Manager

<!-- Page 273 -->

studio-cli plm pipeline
Pipeline commands
Synopsis
Pipeline commands
Options
-h, --help        help for pipeline
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm  - Pipeline Manager
studio-cli plm pipeline create  - Create pipeline from y aml/json deﬁnition ﬁle
studio-cli plm pipeline delete  - Delete pipeline
studio-cli plm pipeline get  - Display the y aml/json for a pipeline
studio-cli plm pipeline get-access-conﬁg  - Display the Access Conﬁg for a pipeline
studio-cli plm pipeline list  - List all pipelines deﬁnitions
studio-cli plm pipeline lock  - Lock pipeline
studio-cli plm pipeline prettify  - Prettify a pipeline or task provided as y aml or json
studio-cli plm pipeline rename-param  - Rename a parameter
studio-cli plm pipeline rename-task  - Rename a T ask
studio-cli plm pipeline unlock  - Unlock pipeline
studio-cli plm pipeline update  - Update pipeline from y aml/json
studio-cli plm pipeline w eave - Weave together a pipeline from y aml/json and a library of local tasks

<!-- Page 274 -->

studio-cli plm pipeline create
Create pipeline from y aml/json deﬁnition ﬁle
Synopsis
Create a pipeline from a y aml/json deﬁnition ﬁle. Y ou can optionally specify a name which ov errides
the name in the y aml/json block.
A pipeline needs an Access Conﬁg to giv e it access to the resources and secrets it needs. Either use the -
a option to provide the name or WRRN of an Access Conﬁg that an admin has assigned to you
studio-cli plm pipeline create [flags]
Options
-a, --access-config string   Provide Access Config name or wrrn
-C, --clone string           clone from pipeline name or id
-d, --data string            Provide json data as a string
-f, --file string            Provide yaml/json via a file
-g, --group string           Provide group name or id (>= version 2309)
-h, --help                   help for create
-n, --name string            Override name of pipeline in yaml/json definition
-L, --task-lib strings       Directories containing tasks
-N, --ui-name string         Override ui.name (human friendly name) of pipeline in
yaml/json definition
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline delete
Delete pipeline

<!-- Page 275 -->

Synopsis
Delete pipeline.
studio-cli plm pipeline delete [flags]
Options
-h, --help          help for delete
-n, --name string   Pipeline name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline get
Display the y aml/json for a pipeline
Synopsis
Display the y aml/json for a pipeline speciﬁed by name.
studio-cli plm pipeline get [flags]
Options
-h, --help             help for get
-t, --include-tasks    Include tasks that were packaged with this pipeline in the
'defineTasks' section
-n, --name string      Pipeline name
-r, --run-number int   Run Number, if set will get the yaml of the pipeline for that
specific run

<!-- Page 276 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline get-access-config
Display the Access Conﬁg for a pipeline
Synopsis
Display the Access Conﬁguration for a pipeline speciﬁed by name or id.
studio-cli plm pipeline get-access-config [flags]
Options
-d, --details       Show details of Access Config
-h, --help          help for get-access-config
-n, --name string   pipeline name or id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands

<!-- Page 277 -->

studio-cli plm pipeline list
List all pipelines deﬁnitions
Synopsis
List all the av ailable pipelines deﬁnitions
studio-cli plm pipeline list [flags]
Options
-h, --help          help for list
-l, --limit int     Limit number of pipelines to return. Values equal to 0 will return
all pipelines. (default 10)
-n, --name string   Pipeline name
-o, --offset int    Return results starting at this offset (default 1)
-u, --user string   Created by user
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline lock
Lock pipeline
Synopsis
Lock speciﬁed pipeline. Only someone with ‘lead’ role in this pipeline, or an admin, can modify a
locked pipeline.
studio-cli plm pipeline lock [flags]

<!-- Page 278 -->

Options
-h, --help          help for lock
-n, --name string   Pipeline name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline prettify
Prettify a pipeline or task provided as y aml or json
Synopsis
Prettify a pipeline or task provided as y aml or json.
studio-cli plm pipeline prettify [flags]
Options
-d, --data string   Provide json data as a string
-f, --file string   Provide yaml/json via a file
-h, --help          help for prettify
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 279 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline rename-param
Rename a parameter
Synopsis
Rename a parameter by entering the old name and the new name of the parameter you w ant to update.
studio-cli plm pipeline rename-param [flags]
Options
-f, --file string             Provide yaml/json via a file
-h, --help                    help for rename-param
-n, --name string             Pipeline name
--new-param-name string   Parameter new name
--old-param-name string   Parameter old name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline rename-task
Rename a T ask

<!-- Page 280 -->

Synopsis
Rename a task by entering the old name and the new name of the task you w ant to update.
studio-cli plm pipeline rename-task [flags]
Options
-f, --file string            Provide yaml/json via a file
-h, --help                   help for rename-task
-n, --name string            Pipeline name
--new-task-name string   Task new name
--old-task-name string   Task old name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline unlock
Unlock pipeline
Synopsis
Unlock speciﬁed pipeline.
studio-cli plm pipeline unlock [flags]
Options
-h, --help          help for unlock
-n, --name string   Pipeline name

<!-- Page 281 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline update
Update pipeline from y aml/json
Synopsis
Update a pipeline from a y aml/json deﬁnition speciﬁed by –data OR –ﬁle
studio-cli plm pipeline update [flags]
Options
-C, --clone string       clone from pipeline name or id
-d, --data string        Provide json data as a string
--dry-run            Simulate an update
-f, --file string        Provide yaml/json via a file
-h, --help               help for update
-n, --name string        Name of pipeline
-L, --task-lib strings   Directories containing tasks
-N, --ui-name string     Override ui.name (human friendly name) of pipeline in yaml/json
definition
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 282 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands
studio-cli plm pipeline weave
Weave together a pipeline from y aml/json and a library of local tasks
Synopsis
Weave together a pipeline from y aml/json and a library of local tasks, and output the resulting pipeline
as yaml/json
studio-cli plm pipeline weave [flags]
Options
-C, --clone string       clone from pipeline name or id
-d, --data string        Provide json data as a string
-f, --file string        Provide yaml/json via a file
-h, --help               help for weave
-n, --name string        Override name of pipeline in yaml/json definition
-L, --task-lib strings   Directories containing tasks
-N, --ui-name string     Override ui.name (human friendly name) of pipeline in yaml/json
definition
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm pipeline  - Pipeline commands

<!-- Page 283 -->

studio-cli plm resource
Pipeline resource commands
Synopsis
Pipeline resource commands.
Options
-a, --access-config string   name or Wind River Resource Name (WRRN) of Access Config
-h, --help                   help for resource
-q, --jq string              jq query string
-p, --pipeline string        name or id of pipeline
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm  - Pipeline Manager
studio-cli plm resource assign  - Assign resource to pipeline or Access Conﬁg
studio-cli plm resource list  - Display list of resources av ailable to an Access Conﬁg or pipeline
studio-cli plm resource revoke  - Revoke resource from pipeline or Access Conﬁg
studio-cli plm resource assign
Assign resource to pipeline or Access Conﬁg
Synopsis
Assign resource to pipeline or Access Conﬁg
studio-cli plm resource assign [flags]

<!-- Page 284 -->

Options
-h, --help               help for assign
-r, --role-name string   Name for RBAC role
-w, --wrrn string        Wind River resource number
Options inherited from parent commands
-a, --access-config string   name or Wind River Resource Name (WRRN) of Access Config
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        name or id of pipeline
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm resource  - Pipeline resource commands
studio-cli plm resource list
Display list of resources av ailable to an Access Conﬁg or pipeline
Synopsis
Display y aml/json formatted list of resources that w ere created with the speciﬁed accessConﬁg or
pipeline
studio-cli plm resource list [flags]
Options
--component-category string   Component Category
--component-type string       Component Type
-h, --help                        help for list
--resource-category string    Resource Category
--resource-type string        Resource Type
Options inherited from parent commands
-a, --access-config string   name or Wind River Resource Name (WRRN) of Access Config
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests

<!-- Page 285 -->

--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        name or id of pipeline
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm resource  - Pipeline resource commands
studio-cli plm resource revoke
Revoke resource from pipeline or Access Conﬁg
Synopsis
Revoke resource from pipeline or Access Conﬁg
studio-cli plm resource revoke [flags]
Options
-h, --help          help for revoke
-w, --wrrn string   Wind River resource number
Options inherited from parent commands
-a, --access-config string   name or Wind River Resource Name (WRRN) of Access Config
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        name or id of pipeline
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm resource  - Pipeline resource commands

<!-- Page 286 -->

studio-cli plm run
Pipeline run commands
Synopsis
Pipeline run commands.
Options
-h, --help        help for run
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm  - Pipeline Manager
studio-cli plm run cancel  - Cancel a pipeline run
studio-cli plm run ev ents - Display pipeline run ev ents
studio-cli plm run get  - Display the y aml/json for a pipeline run
studio-cli plm run list  - List all runs of a pipeline
studio-cli plm run log  - Display pipeline run log details
studio-cli plm run start  - Start a pipeline execution run
studio-cli plm run cancel
Cancel a pipeline run
Synopsis
Cancel a pipeline run.
studio-cli plm run cancel [flags]

<!-- Page 287 -->

Options
-h, --help          help for cancel
-i, --id string     Pipeline id to use instead of name
-n, --name string   Pipeline name
-r, --run int       Run number
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm run  - Pipeline run commands
studio-cli plm run events
Display pipeline run ev ents
Synopsis
Display pipeline run ev ents.
studio-cli plm run events [flags]
Options
-h, --help          help for events
-i, --id string     Pipeline id to use instead of name
-n, --name string   Pipeline name
-r, --run int       Run number
-t, --task string   Task name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string

<!-- Page 288 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm run  - Pipeline run commands
studio-cli plm run get
Display the y aml/json for a pipeline run
Synopsis
Display the y aml/json information for a pipeline run speciﬁed by pipeline name and run number. If
you w ant to get the pipeline y aml of a speciﬁc run use the command studio-cli plm pipeline get -n
pipeline-name -r run-number
studio-cli plm run get [flags]
Options
-h, --help          help for get
-i, --id string     Pipeline id to use instead of name
-n, --name string   Pipeline name
-r, --run int       Run number
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm run  - Pipeline run commands

<!-- Page 289 -->

studio-cli plm run list
List all runs of a pipeline
Synopsis
List all runs of a pipeline
studio-cli plm run list [flags]
Options
-c, --created-by string   User that created the run
-h, --help                help for list
-i, --id string           Pipeline id to use instead of name
-l, --limit int           Limit number of runs to return. Values less or equal to 0 will
return all runs. (default 10)
-n, --name string         Pipeline name
-o, --offset int          Return results starting at this offset (default 1)
-s, --status string       Status of the runs
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm run  - Pipeline run commands
studio-cli plm run log
Display pipeline run log details
Synopsis
Display pipeline run log details.
studio-cli plm run log [flags]

<!-- Page 290 -->

Options
-F, --follow        Specify if the logs should be streamed.
-h, --help          help for log
-i, --id string     Pipeline id to use instead of name
-n, --name string   Pipeline name
-Q, --quiet         Do not print a header before each section of the output
-r, --run int       Run number
-s, --step string   Step name
-t, --task string   Task name (required if --stepname is specified)
-T, --timestamp     Specify if logs should be show with the respective timestamp.
(default true)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm run  - Pipeline run commands
studio-cli plm run start
Start a pipeline execution run
Synopsis
Start a pipeline execution run.
studio-cli plm run start [flags]
Options
-C, --clone string             Clone from pipeline name or id
-c, --config stringArray       Pipeline config settings (e.g. --config project=alpha)
-d, --data string              Provide pipeline definition as a string (overrides the
stored pipeline)
-e, --env stringArray          Pipeline environment settings (e.g. --env PROJECT=alpha)
-f, --file string              Provide pipeline definition via a yaml/json file
(overrides the stored pipeline)
-F, --follow                   Run and stream logs until run has completed

<!-- Page 291 -->

--from-failure             Reuse results of successful tasks from previous run (only
when --rerun is provided)
-G, --gen-baked-run-config     Do not run, just output a self-contained Run Config file
that can be used with the API or with the -R (--run-config) option in future
-g, --gen-run-config           Do not run, just output a Run Config file that can be used
with the -R (--run-config) option in future
-h, --help                     help for start
-i, --id string                Pipeline id to use instead of name
-n, --name string              Pipeline name
-p, --param stringArray        Pipeline parameter settings (e.g. --param project=alpha)
-N, --rerun string             Run number or path to run JSON/YAML file to rerun
-R, --run-config string        Run configuration yaml/json file
--start-task stringArray   Always rerun (i.e. do not reuse results for) this and
following tasks
--stop-task stringArray    Do not include this task, or following tasks in the run
-L, --task-lib strings         Directory containing tasks (only when --data or --file is
provided)
-t, --tasks strings            Tasks to enable or disable (e.g. --tasks Scan=false)
-T, --timestamp                Show logs timestamp. Works with 'follow' flag. (default
true)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm run  - Pipeline run commands
studio-cli plm secret
Pipeline secret commands
Synopsis
Pipeline secret commands.
Options
-a, --access-config string   Access Config name or Wind River Resource Name (WRRN)
-h, --help                   help for secret

<!-- Page 292 -->

-q, --jq string              jq query string
-p, --pipeline string        pipeline name or id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm  - Pipeline Manager
studio-cli plm secret create  - Create secret from options and/or y aml/json
studio-cli plm secret delete  - Delete secret with speciﬁed name
studio-cli plm secret get  - Get metadata about secret with speciﬁed name
studio-cli plm secret list  - List metadata about secrets for pipeline or Access Conﬁg
studio-cli plm secret update  - Update secret from options and/or y aml/json
studio-cli plm secret create
Create secret from options and/or y aml/json
Synopsis
Create a secret from the giv en options and/or a y aml/json deﬁnition. Command line options ov erride
ﬁle settings.
studio-cli plm secret create [flags]
Options
-d, --data string            Provide json data as a string
-f, --file string            Provide yaml/json via a file
-g, --group string           Provide group name or id (>= version 2403)
-h, --help                   help for create
--kv stringArray         Specify secret as Key=Value or Key=@ValueFromFile. May occur
multiple times
-n, --name string            Name of secret
--password string        Together with --username, create a credentials secret
suitable for use with studio-cli
-N, --property-name string   Resource property to attach this secret to

<!-- Page 293 -->

-w, --resource-wrrn string   WRRN of resource to attach this secret to
--template string        Vault template to use to render multi-key secret
--username string        Together with --password, create a credentials secret
suitable for use with studio-cli
--value string           Value of secret specified as string or @file
Options inherited from parent commands
-a, --access-config string   Access Config name or Wind River Resource Name (WRRN)
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        pipeline name or id
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm secret  - Pipeline secret commands
studio-cli plm secret delete
Delete secret with speciﬁed name
Synopsis
Delete secret with speciﬁed name
studio-cli plm secret delete [flags]
Options
-h, --help          help for delete
-n, --name string   Name of secret
Options inherited from parent commands
-a, --access-config string   Access Config name or Wind River Resource Name (WRRN)
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode

<!-- Page 294 -->

--output                 Set Output Format: [json|yaml]
-p, --pipeline string        pipeline name or id
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm secret  - Pipeline secret commands
studio-cli plm secret get
Get metadata about secret with speciﬁed name
Synopsis
Get metadata about secret with speciﬁed name in y aml/json format
studio-cli plm secret get [flags]
Options
-h, --help          help for get
-n, --name string   Name of secret
Options inherited from parent commands
-a, --access-config string   Access Config name or Wind River Resource Name (WRRN)
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        pipeline name or id
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm secret  - Pipeline secret commands
studio-cli plm secret list
List metadata about secrets for pipeline or Access Conﬁg

<!-- Page 295 -->

Synopsis
List metadata about secrets for pipeline or Access Conﬁg
studio-cli plm secret list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
-a, --access-config string   Access Config name or Wind River Resource Name (WRRN)
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        pipeline name or id
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm secret  - Pipeline secret commands
studio-cli plm secret update
Update secret from options and/or y aml/json
Synopsis
Update a secret from the giv en option and/or a y aml/json deﬁnition. Command line options ov erride
ﬁle settings.
studio-cli plm secret update [flags]
Options
-d, --data string            Provide json data as a string
-f, --file string            Provide yaml/json via a file
-h, --help                   help for update
--kv stringArray         Specify secret as Key=Value or Key=@ValueFromFile. May occur
multiple times
-n, --name string            Name of secret
--password string        Together with --username, create a credentials secret

<!-- Page 296 -->

suitable for use with studio-cli
-N, --property-name string   Resource property to attach this secret to
-w, --resource-wrrn string   WRRN of resource to attach this secret to
--template string        Vault template to use to render multi-key secret
--username string        Together with --password, create a credentials secret
suitable for use with studio-cli
--value string           Value of secret specified as string or @file
Options inherited from parent commands
-a, --access-config string   Access Config name or Wind River Resource Name (WRRN)
--debughttp              Print information for http transactions and timings
--debughttp2             Print extended debug data for http requests
--debughttp3             Print extended debug data for http responses
--debughttp4             Pretty print debug data for http responses and requests.
-q, --jq string              jq query string
--non-interactive        Disable all interactive mode
--output                 Set Output Format: [json|yaml]
-p, --pipeline string        pipeline name or id
--raw                    Strip single result jq queries of quotes
--totpcode string        TOTP code
## See Also
studio-cli plm secret  - Pipeline secret commands
studio-cli plm task
Pipeline task commands
Synopsis
Pipeline task commands.
Options
-h, --help        help for task
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 297 -->

## See Also
studio-cli plm  - Pipeline Manager
studio-cli plm task create  - Create task from y aml/json
studio-cli plm task delete  - Delete task
studio-cli plm task get  - Display the y aml/json for a task
studio-cli plm task group  - Task group commands
studio-cli plm task list  - List all the task deﬁnitions
studio-cli plm task lock  - Lock task
studio-cli plm task unlock  - Unlock task
studio-cli plm task update  - Update task from y aml/json
studio-cli plm task upsert  - Create tasks from y aml/json from speciﬁed directory
studio-cli plm task create
Create task from y aml/json
Synopsis
Create a task from a y aml/json deﬁnition and group (name or id). Y ou can optionally specify a name,
uiName (human friendly name), category, and v ersion each of which ov errides ﬁelds in the provided
task deﬁnition.which ov errides the name in the y aml/json block.
studio-cli plm task create [flags]
Options
-c, --category string   Override 'category' in task definition
-d, --data string       Provide task definition as json string
-f, --file string       Provide task definition via a yaml/json file
-g, --group string      Provide group name or id (>= version 2309)
-h, --help              help for create
-n, --name string       Override 'name' in task definition
-N, --ui-name string    Override 'ui.name' (human friendly name) in task definition
-v, --version string    Override 'version' in task definition
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 298 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task  - Pipeline task commands
studio-cli plm task delete
Delete task
Synopsis
Delete pipeline manager task.
studio-cli plm task delete [flags]
Options
-c, --category string   Provide a Task category
-h, --help              help for delete
-i, --id string         Task id to use instead of name
-n, --name string       Task name
-v, --version string    Provide a Task version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task  - Pipeline task commands
studio-cli plm task get
Display the y aml/json for a task

<!-- Page 299 -->

Synopsis
Display the y aml/json for a task speciﬁed by name.
studio-cli plm task get [flags]
Options
-c, --category string   Provide a Task category
-h, --help              help for get
-i, --id string         Task id to use instead of name
-n, --name string       Task name (or [category/]name[@version]
-v, --version string    Provide a Task version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task  - Pipeline task commands
studio-cli plm task group
Task group commands
Synopsis
Task group commands.
Options
-h, --help        help for group
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 300 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task  - Pipeline task commands
studio-cli plm task group assign  - Assign group of users access to task
studio-cli plm task group revoke  - Revoke group of users access to task
studio-cli plm task group assign
Assign group of users access to task
Synopsis
Assign group of users access to task.
studio-cli plm task group assign [flags]
Options
-c, --category string     Provide a Task category
-u, --group-id string     Group id
-g, --group-name string   Group name
-h, --help                help for assign
-i, --id string           Task id to use instead of name
-n, --name string         Task name (or [category/]name[@version]
-r, --role-name string    Role (viewer, tester, editor, or lead) (default "viewer")
-v, --version string      Provide a Task version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 301 -->

## See Also
studio-cli plm task group  - Task group commands
studio-cli plm task group revoke
Revoke group of users access to task
Synopsis
Revoke group of users access to task.
studio-cli plm task group revoke [flags]
Options
-c, --category string     Provide a Task category
-u, --group-id string     Group id
-g, --group-name string   Group name
-h, --help                help for revoke
-i, --id string           Task id to use instead of name
-n, --name string         Task name (or [category/]name[@version]
-v, --version string      Provide a Task version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task group  - Task group commands
studio-cli plm task list
List all the task deﬁnitions

<!-- Page 302 -->

Synopsis
List all the av ailable task deﬁnitions
studio-cli plm task list [flags]
Options
-c, --category string   Task category
-h, --help              help for list
-l, --limit int         Limit number of tasks to return. Values equal to 0 will return
all tasks. (default 10)
-n, --name string       Task name
-o, --offset int        Return results starting at this offset (default 1)
-N, --ui-name string    Task display(ui) name
-u, --user string       Created by user
-v, --version string    Task version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task  - Pipeline task commands
studio-cli plm task lock
Lock task
Synopsis
Lock speciﬁed task. Only the creator of a task, or an admin, can modify a locked task.
studio-cli plm task lock [flags]
Options
-c, --category string   Provide a Task category
-h, --help              help for lock

<!-- Page 303 -->

-i, --id string         Task id to use instead of name
-n, --name string       Task name (or [category/]name[@version]
-v, --version string    Provide a Task version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task  - Pipeline task commands
studio-cli plm task unlock
Unlock task
Synopsis
Unlock speciﬁed task.
studio-cli plm task unlock [flags]
Options
-c, --category string   Provide a Task category
-h, --help              help for unlock
-i, --id string         Task id to use instead of name
-n, --name string       Task name (or [category/]name[@version]
-v, --version string    Provide a Task version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 304 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task  - Pipeline task commands
studio-cli plm task update
Update task from y aml/json
Synopsis
Update a task from a y aml/json deﬁnition speciﬁed by –data OR –ﬁle
studio-cli plm task update [flags]
Options
-c, --category string   Provide task category, if is not provided the value will be
picked from yaml/json
-d, --data string       Provide json data as a string
-f, --file string       Provide yaml/json via a file
-h, --help              help for update
-n, --name string       Provide task name, if is not provided the value will be picked
from yaml/json
-v, --version string    Provide task version, if is not provided the value will be picked
from yaml/json
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task  - Pipeline task commands

<!-- Page 305 -->

studio-cli plm task upsert
Create tasks from y aml/json from speciﬁed directory
Synopsis
Create tasks from y aml/json from speciﬁed directory
studio-cli plm task upsert [flags]
Options
-g, --group string       Provide group name or id (It is mandatory to create new tasks)
-h, --help               help for upsert
-L, --task-lib strings   Directory containing tasks
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm task  - Pipeline task commands
studio-cli plm trigger
Pipeline trigger commands
Synopsis
Pipeline trigger commands.
Options
-h, --help        help for trigger
-q, --jq string   jq query string

<!-- Page 306 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm  - Pipeline Manager
studio-cli plm trigger create  - Create trigger from y aml/json
studio-cli plm trigger delete  - Delete trigger
studio-cli plm trigger get  - Display the y aml/json for a trigger
studio-cli plm trigger list  - List all trigger deﬁnitions
studio-cli plm trigger refresh-secret  - Refreshes a trigger secret
studio-cli plm trigger update  - Update trigger from y aml/json
studio-cli plm trigger create
Create trigger from y aml/json
Synopsis
Create a trigger from a y aml/json deﬁnition. Y ou can optionally specify a name which ov errides the
name in the deﬁnition ﬁle. A sample Y AML ﬁle is as follows.
name: my-pipeline-trigger3 when: ev entSource: scheduled cronSchedule: “0 * * * *” run: pipeline: my-
pipeline
studio-cli plm trigger create [flags]
Options
-d, --data string       Provide trigger definition as a json string
-f, --file string       Provide trigger definition as a yaml/json file
-h, --help              help for create
-n, --name string       Override name of trigger in trigger definition
-p, --pipeline string   Override name or id of pipeline in trigger definition

<!-- Page 307 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm trigger  - Pipeline trigger commands
studio-cli plm trigger delete
Delete trigger
Synopsis
Delete pipeline manager trigger.
studio-cli plm trigger delete [flags]
Options
-h, --help          help for delete
-n, --name string   Trigger name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm trigger  - Pipeline trigger commands

<!-- Page 308 -->

studio-cli plm trigger get
Display the y aml/json for a trigger
Synopsis
Display the y aml/json for a trigger speciﬁed by name.
studio-cli plm trigger get [flags]
Options
-h, --help          help for get
-n, --name string   Trigger name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm trigger  - Pipeline trigger commands
studio-cli plm trigger list
List all trigger deﬁnitions
Synopsis
List all the av ailable trigger deﬁnitions
studio-cli plm trigger list [flags]
Options
-c, --created-by string      User that created the trigger
-e, --event-source string    Kind of trigger (scheduled or webhook)
-h, --help                   help for list
-l, --limit int              Limit number of triggers to return. Values equal to 0 will
return all triggers. (default 10)

<!-- Page 309 -->

-d, --modified-date string   Last modified date
-n, --name string            Trigger Name
-o, --offset int             Return results starting at this offset (default 1)
-p, --pipeline string        Pipeline name or id
-t, --timezone string        A timezone
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm trigger  - Pipeline trigger commands
studio-cli plm trigger refresh-secret
Refreshes a trigger secret
Synopsis
Refreshes a trigger secret.
studio-cli plm trigger refresh-secret [flags]
Options
-h, --help          help for refresh-secret
-n, --name string   Name of the trigger
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 310 -->

## See Also
studio-cli plm trigger  - Pipeline trigger commands
studio-cli plm trigger update
Update trigger from y aml/json
Synopsis
Update a trigger from a y aml/json deﬁnition speciﬁed by –data OR –ﬁle
studio-cli plm trigger update [flags]
Options
-d, --data string   Provide json data as a string
-f, --file string   Provide yaml/json via a file
-h, --help          help for update
-n, --name string   Name of trigger
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli plm trigger  - Pipeline trigger commands
studio-cli portal
Main Control Program
Synopsis
Main Control Program

<!-- Page 311 -->

Options
-h, --help        help for portal
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli portal http  - Submit calls directly to the REST API
studio-cli portal license  - Mcp license command group
studio-cli portal list  - List installed WR Studio components and their v ersions
studio-cli portal resource  - Mcp resource command group
studio-cli portal storage  - Mcp storage command group
studio-cli portal http
Submit calls directly to the REST API
Synopsis
The http subcommand works like a curl command where you specify the type of request and data via
the –data or –bodyﬁle argument if it is a POST or PUT call.
If the ﬁrst character of the -d/–data argument is an @ character, what follows will be used as a ﬁlename.
studio-cli portal http [flags]
Options
-b, --bodyfile string   Data from file for a POST or PUT type call
-d, --data string       Data for a POST or PUT type call
-e, --endpoint string   end point (e.g. /pipeline/list)
-h, --help              help for http
-t, --type              http request type, [DELETE|GET|HEAD|POST|PUT]

<!-- Page 312 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal  - Main Control Program
studio-cli portal license
Mcp license command group
Synopsis
Mcp license command group.
Options
-h, --help        help for license
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 313 -->

## See Also
studio-cli portal  - Main Control Program
studio-cli portal license add  - Add New License
studio-cli portal license assign  - Assign a License to an User
studio-cli portal license create  - Create a License Audit Record on Studio
studio-cli portal license get  - Get Current Studio License Info
studio-cli portal license report  - Display license state information
studio-cli portal license revoke  - Revoke a License from an User
studio-cli portal license add
Add New License
Synopsis
Add New License
studio-cli portal license add [flags]
Options
-h, --help             help for add
-l, --licfile string   License File Path
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal license  - Mcp license command group

<!-- Page 314 -->

studio-cli portal license assign
Assign a License to an User
Synopsis
Assign a License to an User
studio-cli portal license assign [flags]
Options
-h, --help              help for assign
-i, --id string         Id of the user to be assigned to the license.
-n, --username string   User name to be assigned to the license.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal license  - Mcp license command group
studio-cli portal license create
Create a License Audit Record on Studio
Synopsis
Create a License Audit Record on Studio
studio-cli portal license create [flags]
Options
-b, --body string   Name of the license entitlement accessed
-h, --help          help for create

<!-- Page 315 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal license  - Mcp license command group
studio-cli portal license get
Get Current Studio License Info
Synopsis
Get Current Studio License Info
studio-cli portal license get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal license  - Mcp license command group

<!-- Page 316 -->

studio-cli portal license report
Display license state information
Synopsis
Display license state information for all the users on the system or query for a speciﬁc user.
studio-cli portal license report [flags]
Options
-h, --help            help for report
-l, --limit int       Limit project list to X entries per page (use 0 for all) (default
10)
-p, --page int        Page number of listing when using a limit (default 1)
-s, --search string   Search by name or id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal license  - Mcp license command group
studio-cli portal license revoke
Revoke a License from an User
Synopsis
Revoke a License from an User
studio-cli portal license revoke [flags]
Options
-h, --help              help for revoke
-i, --id string         Id of the user to be revoked to the license.

<!-- Page 317 -->

-n, --username string   User name to be revoked to the license.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal license  - Mcp license command group
studio-cli portal list
List installed WR Studio components and their v ersions
Synopsis
List installed WR Studio components and their v ersions.
studio-cli portal list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal  - Main Control Program

<!-- Page 318 -->

studio-cli portal resource
Mcp resource command group
Synopsis
The resourcerequest command interacts with the MCP to adjust the compute resources av ailable to the
deployment.
Options
-h, --help        help for resource
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal  - Main Control Program
studio-cli portal resource create  - Create a resource request
studio-cli portal resource delete  - Delete a resource request
studio-cli portal resource edit  - Edit a resource request
studio-cli portal resource list  - Display the activ e resource requests
studio-cli portal resource list-nodetypes  - Display the v alid combinations of size and purpose
studio-cli portal resource sync  - Synchronize Compute Types.
studio-cli portal resource create
Create a resource request
Synopsis
Create a resource request.
studio-cli portal resource create [flags]

<!-- Page 319 -->

Options
--description string   ComputeSet description
-h, --help                 help for create
--max uint             Maximum #number of nodes
--min uint             Minimum #number of nodes
--name string          Name or Id of resource request (default "n")
--purpose string       Purpose of nodes (selected from listnodetypes)
--size string          Size of nodes (selected from listnodetypes)
--spares uint          Number of spares nodes to keep online
--tag stringArray      Only allow, <some key>:<some value> without white spaces.
- usage example: --tag key1:value1 --tag key2:value2

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal resource  - Mcp resource command group
studio-cli portal resource delete
Delete a resource request
Synopsis
Delete a resource request.
studio-cli portal resource delete [flags]
Options
-h, --help          help for delete
--max uint      Maximum #number of nodes
--min uint      Minimum #number of nodes
--name string   Name or Id of resource request (default "n")
--spares uint   Number of spares nodes to keep online

<!-- Page 320 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal resource  - Mcp resource command group
studio-cli portal resource edit
Edit a resource request
Synopsis
Edit a resource request.
studio-cli portal resource edit [flags]
Options
--description string   ComputeSet description
-h, --help                 help for edit
--max uint             Maximum #number of nodes
--min uint             Minimum #number of nodes
--name string          Name or Id of resource request (default "n")
--spares uint          Number of spares nodes to keep online
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 321 -->

## See Also
studio-cli portal resource  - Mcp resource command group
studio-cli portal resource list
Display the activ e resource requests
Synopsis
Display the activ e resource requests.
studio-cli portal resource list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal resource  - Mcp resource command group
studio-cli portal resource list-nodetypes
Display the v alid combinations of size and purpose
Synopsis
Display the v alid combinations of size and purpose.
studio-cli portal resource list-nodetypes [flags]

<!-- Page 322 -->

Options
-h, --help   help for list-nodetypes
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal resource  - Mcp resource command group
studio-cli portal resource sync
Synchronize Compute Types.
Synopsis
Synchronize Compute Types.
studio-cli portal resource sync [flags]
Options
-h, --help   help for sync
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 323 -->

## See Also
studio-cli portal resource  - Mcp resource command group
studio-cli portal storage
Mcp storage command group
Synopsis
The storagemanager command interacts with the MCP to the storage resources av ailable.
Options
-h, --help        help for storage
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal  - Main Control Program
studio-cli portal storage create  - Create storage resources
studio-cli portal storage delete  - Delete storage resources
studio-cli portal storage edit  - Edit PV and PVC capacity
studio-cli portal storage search  - Search resource
studio-cli portal storage create
Create storage resources
Synopsis
Create storage resources

<!-- Page 324 -->

studio-cli portal storage create [flags]
Options
-a, --accessModes stringArray   Access mode for storage resources. Default value if not
provided: ReadWriteOnce
- usage example: --accessModes ReadWriteOnce

-c, --capacity string           Storage resource capacity
-C, --component string          Component name requesting the resources, e.g. mcp
-d, --description string        Storage resources description
-f, --fsType string             File System type for EFS storage resources. Default value
if not provided: ext4 (default "ext4")
-g, --group string              Group name to be associated to storage resource in RBAC
-h, --help                      help for create
-m, --mountOptions string       Mount options for EFS storage resources
-N, --name string               Name or Id of storage resources
-n, --namespace string          Namespace where PVC will be created
-r, --reclaimPolicy string      Reclaim policy for storage resources. Default value if
not provided: Delete (default "Delete")
-s, --storageClassName string   Storage class name for EBS or EFS types.
- usage example: --storageClassName efs-sc
-z, --volumeAZ string           Availability Zone for EBS storage resources
-v, --volumeMode string         Volume mode for storage resources. Default value if not
provided: Filesystem (default "Filesystem")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal storage  - Mcp storage command group
studio-cli portal storage delete
Delete storage resources
Synopsis
Delete storage resources.

<!-- Page 325 -->

studio-cli portal storage delete [flags]
Options
-h, --help               help for delete
-n, --namespace string   Namespace where the Persistent Volume Claim is assigned
-v, --pvName string      Name of Persistent Volume
-c, --pvcName string     Name of Persistent Volume Claim
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal storage  - Mcp storage command group
studio-cli portal storage edit
Edit PV and PVC capacity
Synopsis
Edit PV and PVC capacity.
studio-cli portal storage edit [flags]
Options
-s, --capacity string    Persistent Volume capacity requested in GiB
-h, --help               help for edit
-n, --namespace string   Namespace where the Persistent Volume Claim is assigned
-v, --pvName string      Name of Persistent Volume
-c, --pvcName string     Name of Persistent Volume Claim
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 326 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal storage  - Mcp storage command group
studio-cli portal storage search
Search resource
Synopsis
Search resource
studio-cli portal storage search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth

<!-- Page 327 -->

- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system

-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli portal storage  - Mcp storage command group
studio-cli ram
Ram commands (previous rbac)

<!-- Page 328 -->

Synopsis
Ram commands (previous rbac).
Options
-h, --help        help for ram
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli ram component  - Component manager
studio-cli ram component-template  - Component-template manager
studio-cli ram environment  - Environment manager
studio-cli ram location  - Location manager
studio-cli ram resource  - Resource manager
studio-cli ram resource-template  - ResourceT emplate manager
studio-cli ram tag  - Tag manager
studio-cli ram component
Component manager
Synopsis
Component manager
Options
-h, --help        help for component
-q, --jq string   jq query string

<!-- Page 329 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram  - Ram commands (previous rbac)
studio-cli ram component create  - Create a new component
studio-cli ram component delete  - Delete a component
studio-cli ram component get-category  - Get all the component categories
studio-cli ram component get-resource  - Get a list of resources related to a component
studio-cli ram component get-state  - Get all the components states
studio-cli ram component get-type  - Get all the component types
studio-cli ram component read  - Read one component by wrrn
studio-cli ram component search  - Search and ﬁlter components
studio-cli ram component update  - Update component
studio-cli ram component-template
Component-template manager
Synopsis
Component-template manager
Options
-h, --help        help for component-template
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode

<!-- Page 330 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram  - Ram commands (previous rbac)
studio-cli ram component-template assign  - Assign a component-template to a component by id and
wrrn.
studio-cli ram component-template create  - Create a new component-template.
studio-cli ram component-template delete  - Delete a component-template by id.
studio-cli ram component-template read  - Read one component-template by id.
studio-cli ram component-template revoke  - Revoke a component-template to a component by id
and wrrn.
studio-cli ram component-template search  - Search and ﬁlter component-templates.
studio-cli ram component-template assign
Assign a component-template to a component by id and wrrn.
Synopsis
Assign a component-template to a component by id and wrrn.
studio-cli ram component-template assign [flags]
Options
-h, --help          help for assign
-i, --id string     Component Template id to be consulted
-t, --type string   Component type, such as gitlab, plm, etc
-w, --wrrn string   Component location unique identifier WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 331 -->

## See Also
studio-cli ram component-template  - Component-template manager
studio-cli ram component-template create
Create a new component-template.
Synopsis
Create a new component-template.
studio-cli ram component-template create [flags]
Options
-d, --description string   Basic description of the template use.
-h, --help                 help for create
-i, --information          Parameter to get more specific information in response
-n, --name string          component-templates name
-s, --state string         state of the component template (default "draft")
-t, --template string      component-templates
-v, --version string       component-templates version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component-template  - Component-template manager
studio-cli ram component-template delete
Delete a component-template by id.

<!-- Page 332 -->

Synopsis
Delete a component-template by id. Delete only if not assigned.
studio-cli ram component-template delete [flags]
Options
-h, --help        help for delete
-i, --id string   Component Template id to be deleted
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component-template  - Component-template manager
studio-cli ram component-template read
Read one component-template by id.
Synopsis
Read one component-template by id.
studio-cli ram component-template read [flags]
Options
-h, --help        help for read
-i, --id string   Component Template id to be consulted
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses

<!-- Page 333 -->

--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component-template  - Component-template manager
studio-cli ram component-template revoke
Revoke a component-template to a component by id and wrrn.
Synopsis
Revoke a component-template to a component by id and wrrn.
studio-cli ram component-template revoke [flags]
Options
-h, --help          help for revoke
-i, --id string     Component Template id to be assigned
-w, --wrrn string   Component location unique identifier WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component-template  - Component-template manager
studio-cli ram component-template search
Search and ﬁlter component-templates.

<!-- Page 334 -->

Synopsis
Search and ﬁlter component-templates. Get all system component-templates if no prams provided.
studio-cli ram component-template search [flags]
Options
-h, --help             help for search
-l, --limit float32    Limit (default 10)
-n, --name string      component-templates name
-p, --page float32     Page (default 1)
-v, --version string   component-templates version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component-template  - Component-template manager
studio-cli ram component create
Create a new component
Synopsis
Create a new component.
studio-cli ram component create [flags]
Options
-c, --category string      Categories used by Studio components to filter when searching
for  components. Use the command to find those in use: studio-cli rbac  component
getCategories
-d, --description string   Enclosed in " if the string includes spaces
-h, --help                 help for create
-l, --location string      The Studio generated wrrn for the location. obtained using

<!-- Page 335 -->

studio-cli rbac location search
-n, --name string          Enclosed in " if the string includes spaces, naming must be
unique
-s, --state string         Permitted Value: [pending|ready|archive|deleted]
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-t, --type string          The type of component. This is critical for the CLI to enable
creating associated resource, the type is used to validate
supported types for studio-cli rbac resource create command
to work:
- vault
- gitlab
- artifacts
- systemRegistry
- deviceRegistry
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp

-k, --unique-data string   Unique Data corresponding to the component. Specified as
string or @file, json object with internal " escaped
single object: "{\"key\":\"value\"}"
multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
required for specific component [ota]:
"uniqueData": {
"apiHostName": "ota.excelfore.dev-eng.wrstudio.cloud",
//use the url to the server
"apiPort": "8443", //use the assigned port for the API
on the server
"certificatePassword": "password",
"devicePort": "9084", //the assigned port for devices
to communicate with the server
"deviceTenancy": "default"
}

<!-- Page 336 -->

-u, --url string           Route to the component - include https:// for the use by the
UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component  - Component manager
studio-cli ram component delete
Delete a component
Synopsis
Delete a component.
studio-cli ram component delete [flags]
Options
-h, --help          help for delete
-w, --wrrn string   Component name space wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 337 -->

## See Also
studio-cli ram component  - Component manager
studio-cli ram component get-category
Get all the component categories
Synopsis
Get all the component categories.
studio-cli ram component get-category [flags]
Options
-h, --help   help for get-category
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component  - Component manager
studio-cli ram component get-resource
Get a list of resources related to a component
Synopsis
Get a list of resources related to a component. Return false if no resource associated with it.
studio-cli ram component get-resource [flags]

<!-- Page 338 -->

Options
-h, --help            help for get-resource
-l, --limit float32   max amount of results to get on a response (default 10)
-p, --page float32    page of results to consult for (default 1)
-w, --wrrn string     Component wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component  - Component manager
studio-cli ram component get-state
Get all the components states
Synopsis
Get all the components states.
studio-cli ram component get-state [flags]
Options
-h, --help   help for get-state
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 339 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component  - Component manager
studio-cli ram component get-type
Get all the component types
Synopsis
Get all the component types.
studio-cli ram component get-type [flags]
Options
-h, --help   help for get-type
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component  - Component manager
studio-cli ram component read
Read one component by wrrn
Synopsis
Read one component by wrrn.

<!-- Page 340 -->

studio-cli ram component read [flags]
Options
-h, --help          help for read
-w, --wrrn string   Component wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component  - Component manager
studio-cli ram component search
Search and ﬁlter components
Synopsis
Search and ﬁlter components.
studio-cli ram component search [flags]
Options
-a, --active               Status of the component - look just for active components
(default true)
-c, --category string      category of the component
-e, --environment string   environment where the component is located
-g, --geo string           geographic location where the component is located
-h, --help                 help for search
-l, --limit float32        max amount of results on a search response (default 10)
-n, --name string          name of the component
-s, --namespace string     namespace where the component is located
-p, --page float32         page to look for - paination params (default 1)
-t, --type string          type of the componen
--url string           URL of the component - will look for full or partial match on
components urls

<!-- Page 341 -->

-u, --username string      To get just the components that a user has access to
-w, --wrrn string          Component WRRN - will look for components that fully or
partial match with the wrrn given
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component  - Component manager
studio-cli ram component update
Update component
Synopsis
Update component.
studio-cli ram component update [flags]
Options
-d, --description string   New description for the Component
-h, --help                 help for update
-n, --name string          New name for the Component
-l, --url string           New url for the Component
-w, --wrrn string          Component wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 342 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram component  - Component manager
studio-cli ram environment
Environment manager
Synopsis
Environment manager
Options
-h, --help        help for environment
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram  - Ram commands (previous rbac)
studio-cli ram environment create  - Create a new component environment
studio-cli ram environment delete  - Delete component environment
studio-cli ram environment get-state  - Get Component Environments states
studio-cli ram environment read  - Read one component environment
studio-cli ram environment search  - Search component environment
studio-cli ram environment update  - Update component environment

<!-- Page 343 -->

studio-cli ram environment create
Create a new component environment
Synopsis
Create a new component environment.
studio-cli ram environment create [flags]
Options
-c, --cloud string          Enclosed in " if the string includes spaces. May include
multiple clouds if the environment spans them
-d, --description string    Enclosed in " if the string includes spaces
-h, --help                  help for create
-n, --name string           Enclosed in " if the string includes spaces, naming must be
unique
-o, --organization string   Enclosed in " if the string includes spaces
-s, --state string          Permitted Value: [pending|ready|suspended|archive|deleted],
when this object is deleted the first time, its state is changed to deleted and it won't
show up in a CLI search but will show up in API search. Second time its deleted it is
deleted from  database
-k, --unique-data string    Json object with internal " escaped, single object: "
{\"key\":\"value\"}", multiple objects: ["{\"key1\":\"value1\"}", "{\"key2\":\"value2\"}"]
-u, --url string            Include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram environment  - Environment manager
studio-cli ram environment delete
Delete component environment

<!-- Page 344 -->

Synopsis
Delete component Environment
studio-cli ram environment delete [flags]
Options
-h, --help          help for delete
-w, --wrrn string   unique environment identifier
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram environment  - Environment manager
studio-cli ram environment get-state
Get Component Environments states
Synopsis
Get Component Environments states.
studio-cli ram environment get-state [flags]
Options
-h, --help   help for get-state
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 345 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram environment  - Environment manager
studio-cli ram environment read
Read one component environment
Synopsis
Read one component environment
studio-cli ram environment read [flags]
Options
-h, --help          help for read
-w, --wrrn string   Wind River Resource Number
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram environment  - Environment manager
studio-cli ram environment search
Search component environment

<!-- Page 346 -->

Synopsis
Search component environment.
studio-cli ram environment search [flags]
Options
-c, --cloud string          Enclosed in " if the string includes spaces. May include
multiple clouds if the environment spans them
-h, --help                  help for search
-l, --limit float32         Limit (default 10)
-n, --name string           Enclosed in " if the string includes spaces, naming must be
unique
-o, --organization string   Enclosed in " if the string includes spaces
-p, --page float32          Page (default 1)
-w, --wrrn string           WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram environment  - Environment manager
studio-cli ram environment update
Update component environment
Synopsis
Update component environment.
studio-cli ram environment update [flags]
Options
-c, --cloud string          Enclosed in " if the string includes spaces. May include
multiple clouds if the environment spans them

<!-- Page 347 -->

-d, --description string    Enclosed in " if the string includes spaces
-h, --help                  help for update
-n, --name string           Enclosed in " if the string includes spaces, naming must be
unique
-o, --organization string   Enclosed in " if the string includes spaces
-s, --state string          Permitted Value: [pending|ready|suspended|archive|deleted],
when this object is deleted the first time, its state is changed to deleted and it won't
show up in a CLI search but will show up in API search. Second time its deleted it is
deleted from  database
-k, --unique-data string    Json object with internal " escaped, single object: "
{\"key\":\"value\"}", multiple objects: ["{\"key1\":\"value1\"}", "{\"key2\":\"value2\"}"]
-u, --url string            Include https:// for the use by the UI when its available
-w, --wrrn string           Wind River Resource Number
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram environment  - Environment manager
studio-cli ram location
Location manager
Synopsis
Location manager
Options
-h, --help        help for location
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 348 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram  - Ram commands (previous rbac)
studio-cli ram location create  - Create a new component location
studio-cli ram location delete  - Delete a component location
studio-cli ram location read  - Get one component location
studio-cli ram location search  - Search all Component locations
studio-cli ram location create
Create a new component location
Synopsis
Create a new component location.
studio-cli ram location create [flags]
Options
-i, --cluster-id string         The unique ID assigned to the Kubernetes cluster by the
cloud platform Obtained using kubectl or cloud command or ui
-c, --cluster-name string       The unique name assigned to the Kubernetes cluster.
Obtained using kubectl or cloud  command or ui
-d, --description string        Enclosed in " if the string includes spaces
-e, --environment-wrrn string   The Studio generated wrrn for the environment. obtained
using studio-cli rbac environment search
-g, --geographic string         Physical location where the instance is located
-h, --help                      help for create
-n, --namespace string          unique namespace identifier of the location
-k, --unique-data string        Json object with internal " escaped, single object: "
{\"key\":\"value\"}", multiple objects: ["{\"key1\":\"value1\"}", "{\"key2\":\"value2\"}"]
-u, --url string                Include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 349 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram location  - Location manager
studio-cli ram location delete
Delete a component location
Synopsis
Delete a component location.
studio-cli ram location delete [flags]
Options
-h, --help          help for delete
-w, --wrrn string   Component location unique identifier WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram location  - Location manager
studio-cli ram location read
Get one component location

<!-- Page 350 -->

Synopsis
Get one component location.
studio-cli ram location read [flags]
Options
-h, --help          help for read
-w, --wrrn string   Component location unique identifier WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram location  - Location manager
studio-cli ram location search
Search all Component locations
Synopsis
Search all Component locations.
studio-cli ram location search [flags]
Options
-i, --cluster-id string        location clusterId property
-c, --cluster-name string      location clusterName property
-e, --environmentWrrn string   location envWrrn property
-g, --geographic string        location geographic property
-h, --help                     help for search
-l, --limit float32            limit of object to look for - max number of items per
response (default 10)
-n, --namespace string         location namespace property
-p, --page float32             page number to look for - endpoint use paginatio (default

<!-- Page 351 -->

1)
-u, --url string               location url property
-w, --wrrn string              location unique identifier
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram location  - Location manager
studio-cli ram resource
Resource manager
Synopsis
Resource manager
Options
-h, --help        help for resource
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 352 -->

## See Also
studio-cli ram  - Ram commands (previous rbac)
studio-cli ram resource check-health  - Endpoint to check if Resource-MS is working.
studio-cli ram resource check-role  - Check if user has matched role and get all user roles
studio-cli ram resource check-user-access  - Get all the components associated to an user and has
access
studio-cli ram resource create  - Create resource
studio-cli ram resource delete  - Delete resource
studio-cli ram resource get  - Get resource
studio-cli ram resource get-category  - Get resource categories
studio-cli ram resource get-state  - Get resource states
studio-cli ram resource get-type  - Get resource types
studio-cli ram resource get-users  - Get resource users
studio-cli ram resource search  - Search resource
studio-cli ram resource update  - Update resource
studio-cli ram resource-template
ResourceT emplate manager
Synopsis
ResourceT emplate manager
Options
-h, --help   help for resource-template
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 353 -->

## See Also
studio-cli ram  - Ram commands (previous rbac)
studio-cli ram resource-template assign  - Assign a resources-template to a component by id and
wrrn.
studio-cli ram resource-template create  - Create a new resources-template.
studio-cli ram resource-template delete  - Delete a resources-template
studio-cli ram resource-template get  - Get a resources-template
studio-cli ram resource-template revoke  - Revoke a resources-template
studio-cli ram resource-template search  - Search a resources-template.
studio-cli ram resource-template assign
Assign a resources-template to a component by id and wrrn.
Synopsis
Assign a resources-template to a component by id and wrrn.
studio-cli ram resource-template assign [flags]
Options
-h, --help          help for assign
-i, --id string     resources Template id to be consulted
-t, --type string   typeOfResourceAssigned
-w, --wrrn string   Component unique identifier WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource-template  - ResourceT emplate manager

<!-- Page 354 -->

studio-cli ram resource-template create
Create a new resources-template.
Synopsis
Create a new resources-template.
studio-cli ram resource-template create [flags]
Options
--description string   resources-templates description
-d, --detailed-info        resources-templates  (default true)
-h, --help                 help for create
-n, --name string          resources-templates name
-s, --state string         resources-templates state (default "draft")
-t, --template string      resources-templates
-v, --version string       resources-templates version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource-template  - ResourceT emplate manager
studio-cli ram resource-template delete
Delete a resources-template
Synopsis
Delete a resources-template
studio-cli ram resource-template delete [flags]

<!-- Page 355 -->

Options
-h, --help        help for delete
-i, --id string   resources Template id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource-template  - ResourceT emplate manager
studio-cli ram resource-template get
Get a resources-template
Synopsis
Get a resources-template
studio-cli ram resource-template get [flags]
Options
-h, --help        help for get
-i, --id string   resources Template id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 356 -->

## See Also
studio-cli ram resource-template  - ResourceT emplate manager
studio-cli ram resource-template revoke
Revoke a resources-template
Synopsis
Revoke a resources-template
studio-cli ram resource-template revoke [flags]
Options
-h, --help          help for revoke
-i, --id string     resources Template id
-w, --wrrn string   Component unique identifier WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource-template  - ResourceT emplate manager
studio-cli ram resource-template search
Search a resources-template.
Synopsis
Search a resources-template.
studio-cli ram resource-template search [flags]

<!-- Page 357 -->

Options
-h, --help             help for search
-i, --id string        id
-l, --limit float32    The max number of items to return - greater than 0 (default 10)
-n, --name string      resources Template name to be consulted
-p, --page float32     Page number  - greater than 0 (default 1)
-v, --version string   resources Template version to be consulted
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource-template  - ResourceT emplate manager
studio-cli ram resource check-health
Endpoint to check if Resource-MS is working.
Synopsis
Endpoint to check if Resource-MS is working.
studio-cli ram resource check-health [flags]
Options
-h, --help   help for check-health
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 358 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource check-role
Check if user has matched role and get all user roles
Synopsis
Check if user has matched role and get all user roles
studio-cli ram resource check-role [flags]
Options
-w, --componentWrrn string   Component wrrn required to consult for results
-h, --help                   help for check-role
-r, --requiredRole string    Target role to which a given user has access to (optional)
-t, --target string          Resource inside the component to which a given user's access
is validated. (optional)
-n, --team string            Used if query is to be refined by a specific team name.
(optional)
-u, --user string            UserId required to consult for results
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager

<!-- Page 359 -->

studio-cli ram resource check-user-access
Get all the components associated to an user and has access
Synopsis
Get all the components associated to an user and has access
studio-cli ram resource check-user-access [flags]
Options
--category string         Category name of the resource. (optional)
--component-wrrn string   WRRN of the component where the resource belongs.
-h, --help                    help for check-user-access
--keys string             Tag keys of the resource. (optional)
--limit float32           (optional) (default 20)
--page float32            (optional) (default 1)
--rbac-role string        Used to check if specific rbacRole is required. (optional)
--resource-name string    Name of the resource. (optional)
--sort-column string      sort column(optional)
--sort-direction string   sort direction(optional)
--state string            State name of the resource. (optional)
--tool-id string          Identifier of the tool within the component. (optional)
--type string             Name of the resource typ (optional)
--url string              Url of the resource. (optional)
--user string             Username
--values string           Tag values of the resource. (optional)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource create
Create resource

<!-- Page 360 -->

Synopsis
Create resource
studio-cli ram resource create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"type":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags":[{"key2":"value1"},
{"key2":"value2"}],
"unique-data":"
{"srcPath\":\"srcPath\",\"destPath\":\"minio/yourBucketName\"}",
"url":"",
"tool-id":""
}
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
-t, --resource-type string    (*) The type of the resource depending on the component.
This is critical for the CLI to enable creating associated resource, the type is used to
validate
supported types for studio-cli rbac resource create
command to work:
- folder/secret when component is vault
- repo/project/branch when component is gitlab
- bucket/folder/object when component is

<!-- Page 361 -->

artifacts
other types to used and will be supported:
- folder/pipeline/freestyle when component is
jenkins
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- systemRegistry
- deviceRegistry
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp

-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          ToolId of the resource, used only when manual flag is true
--unique-data string      Unique metadata needed by the component to configure or
process the resource.
This is unique for each resource and depends on the parent
component.
Specified as string or @file.
For string example, to create different resource in
Artifact, destPath has to begin with minio
bucket:
"{\"destPath\":\"minio/yourBucketName\"}"
folder:
"
{\"destPath\":\"minio/bucketName/yourFolderName\"}"
object: yourSrcPath could be local path or minio path
"
{\"srcPath\":\"yourSrcPath\",\"destPath\":\"minio/yourFolderName/yourFileName\"}"
To create resource in Vault, kvPath should be writable

<!-- Page 362 -->

"{\"kvPath\":\"/kv/kvtest\",
\"kvPair\":\"kvhello=kvworld\"}"
To create resource in Jenkins, configFile should be
writable
"{\"configFile\":\"/yourconfigFilePath\"}"
group or repo: no special data is required
"
{\"gitlabType\":\"gitlabType\",\"srcPath\":\"srcPath\",\"destPath\":\"destPath\"}"
branch: projectId should be a writable path with your
repo name.
"{\"projectId\":
\"yourGitlabId/yourRepoName\",\"gitlab-test\": \"branch-test\"}"
project or subgroup under group: groupId should be the
groupId number
"
{\"groupId\":\"8035\",\"srcPath\":\"srcPath\",\"destPath\":\"destPath\"}"
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource delete
Delete resource
Synopsis
Delete resource
studio-cli ram resource delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual

<!-- Page 363 -->

-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource get
Get resource
Synopsis
Get resource
studio-cli ram resource get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 364 -->

## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource get-category
Get resource categories
Synopsis
Get resource categories.
studio-cli ram resource get-category [flags]
Options
-h, --help   help for get-category
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource get-state
Get resource states
Synopsis
Get resource states.
studio-cli ram resource get-state [flags]

<!-- Page 365 -->

Options
-h, --help   help for get-state
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource get-type
Get resource types
Synopsis
Get resource types.
studio-cli ram resource get-type [flags]
Options
-h, --help   help for get-type
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 366 -->

## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource get-users
Get resource users
Synopsis
Get resource users
studio-cli ram resource get-users [flags]
Options
-h, --help          help for get-users
-q, --jq string     jq query string
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource search
Search resource
Synopsis
Search resource
studio-cli ram resource search [flags]

<!-- Page 367 -->

Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system

-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,

<!-- Page 368 -->

include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager
studio-cli ram resource update
Update resource
Synopsis
Update resource
studio-cli ram resource update [flags]
Options
-e, --editor strings           group name
-h, --help                     help for update
-l, --lead strings             group name
-m, --manual                   default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the rbac2505Controller resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-d, --new-description string   Enclosed in " if the string includes spaces
--new-name string          new name for resource
-n, --none                     To revoke a resource from groups
-s, --state string             New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state

<!-- Page 369 -->

- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
-t, --tester strings           group name
-u, --unique-data string       Unique metadata needed by the component to configure or
process the resource.
This is unique for each resource and depends on the parent
component.
Specified as string or @file.
For string example, to create different resource in
Artifact, destPath has to begin with minio
bucket:
"
{\"artifactsType\":\"bucket\",\"srcPath\":\"srcPath\",\"destPath\":\"minio/yourBucketName\"
}"
folder:
"
{\"artifactsType\":\"folder\",\"srcPath\":\"srcPath\",\"destPath\":\"minio/yourFolderName\"
}"
object: yourSrcPath could be local path or minio path
"
{\"srcPath\":\"yourSrcPath\",\"destPath\":\"minio/yourFolderName/yourFileName\"}"
To create resource in Vault, kvPath should be writable
"{\"kvPath\":\"/kv/kvtest\",
\"kvPair\":\"kvhello=kvworld\"}"
To create resource in Gitlab,
group or repo: no special data is required
"
{\"gitlabType\":\"gitlabType\",\"srcPath\":\"srcPath\",\"destPath\":\"destPath\"}"
branch: projectId should be a writable path with your
repo name.
"{\"projectId\":
\"yourGitlabId/yourRepoName\",\"gitlab-test\": \"branch-test\"}"
project or subgroup under group: groupId should be the
groupId number
"
{\"groupId\":\"8035\",\"srcPath\":\"srcPath\",\"destPath\":\"destPath\"}"
--viewer strings           group name
-w, --wrrn string              wrrn of the resource to update
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 370 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram resource  - Resource manager
studio-cli ram tag
Tag manager
Synopsis
Tag manager
Options
-h, --help        help for tag
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram  - Ram commands (previous rbac)
studio-cli ram tag assign  - Assigns a tag or a list of tags to a resource.
studio-cli ram tag create  - Create a new tag.
studio-cli ram tag delete  - Delete a tag.
studio-cli ram tag get-key  - Get all tags’ keys.
studio-cli ram tag get-v alue - Get all tags’ v alues.
studio-cli ram tag revoke  - Revoke tag to resource.
studio-cli ram tag search  - Search tags.

<!-- Page 371 -->

studio-cli ram tag assign
Assigns a tag or a list of tags to a resource.
Synopsis
Assigns a tag or a list of tags to a resource.
studio-cli ram tag assign [flags]
Options
-h, --help           help for assign
-k, --key string     Tag key
-v, --value string   Tag value
-r, --wrrn string    Resource to assign.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram tag  - Tag manager
studio-cli ram tag create
Create a new tag.
Synopsis
Create a new tag.
studio-cli ram tag create [flags]
Options
-h, --help           help for create
-k, --key string     Tag key

<!-- Page 372 -->

-v, --value string   Tag value
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram tag  - Tag manager
studio-cli ram tag delete
Delete a tag.
Synopsis
Delete a tag.
studio-cli ram tag delete [flags]
Options
-h, --help           help for delete
-k, --key string     Tag key
-v, --value string   Tag value
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 373 -->

## See Also
studio-cli ram tag  - Tag manager
studio-cli ram tag get-key
Get all tags’ keys.
Synopsis
Get all tags’ keys.
studio-cli ram tag get-key [flags]
Options
-h, --help   help for get-key
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram tag  - Tag manager
studio-cli ram tag get-value
Get all tags’ v alues.
Synopsis
Get all tags’ v alues.
studio-cli ram tag get-value [flags]

<!-- Page 374 -->

Options
-h, --help   help for get-value
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram tag  - Tag manager
studio-cli ram tag revoke
Revoke tag to resource.
Synopsis
Revoke tag to resource.
studio-cli ram tag revoke [flags]
Options
-h, --help           help for revoke
-k, --key string     Tag key
-v, --value string   Tag value
-r, --wrrn string    Resource to revoke.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 375 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram tag  - Tag manager
studio-cli ram tag search
Search tags.
Synopsis
.
studio-cli ram tag search [flags]
Options
-h, --help            help for search
-l, --limit float32   Limit Tags list to X entries per page (default: 0 for all) (default
10)
-p, --page float32    Page number of listing when using a limit (default 1)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ram tag  - Tag manager
studio-cli schedule
Schedule (Studio 23.03 and later )

<!-- Page 376 -->

Synopsis
Schedule (Studio 23.03 and later )\n\n
Options
-h, --help   help for schedule
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli schedule apicall  - ApiCall commands
studio-cli schedule apicall
ApiCall commands
Synopsis
ApiCall commands
Options
-h, --help        help for apicall
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 377 -->

## See Also
studio-cli schedule  - Schedule (Studio 23.03 and later )
studio-cli schedule apicall create  - Create schedule job from api call description
studio-cli schedule apicall delete  - Delete schedule job from api call description
studio-cli schedule apicall execution  - Manage Executions of scheduled jobs
studio-cli schedule apicall get  - Gets a schedule job by uuid
studio-cli schedule apicall list  - List all scheduled job from api call
studio-cli schedule apicall update  - Updates schedule job from api call description
studio-cli schedule apicall create
Create schedule job from api call description
Synopsis
Create a schedule job from api call description.
studio-cli schedule apicall create [flags]
Options
-c, --cron string          Provide cron job configuration
-d, --data string          Data for a POST or PUT type call
-s, --description string   Brief description of the scheduled job
-e, --endpoint string      end point (e.g. /schedule/create)
-h, --help                 help for create
-n, --name string          Scheduled Job Name
-p, --params strings       Call parameters
-t, --type                 http request type, [DELETE|GET|HEAD|POST|PUT]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 378 -->

## See Also
studio-cli schedule apicall  - ApiCall commands
studio-cli schedule apicall delete
Delete schedule job from api call description
Synopsis
Delete a schedule job from api call description.
studio-cli schedule apicall delete [flags]
Options
-h, --help          help for delete
-u, --uuid string   Scheduled Job UUID
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli schedule apicall  - ApiCall commands
studio-cli schedule apicall execution
Manage Executions of scheduled jobs
Synopsis
Manage Executions of scheduled jobs
studio-cli schedule apicall execution [flags]

<!-- Page 379 -->

Options
-h, --help   help for execution
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli schedule apicall  - ApiCall commands
studio-cli schedule apicall execution get  - Gets executions of an scheduled job by uuid
studio-cli schedule apicall execution get
Gets executions of an scheduled job by uuid
Synopsis
Gets executions of an scheduled job by uuid
studio-cli schedule apicall execution get [flags]
Options
-e, --end-time int     Filter executions by endtTime unix epoch.
-h, --help             help for get
-l, --quantity int     How many executions to retrieve.
-s, --start-time int   Filter executions by startTime unix epoch.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 380 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli schedule apicall execution  - Manage Executions of scheduled jobs
studio-cli schedule apicall get
Gets a schedule job by uuid
Synopsis
Gets a scheduled job using uuid
studio-cli schedule apicall get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli schedule apicall  - ApiCall commands
studio-cli schedule apicall list
List all scheduled job from api call
Synopsis
List all schedule job from api call description.

<!-- Page 381 -->

studio-cli schedule apicall list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli schedule apicall  - ApiCall commands
studio-cli schedule apicall update
Updates schedule job from api call description
Synopsis
Updates a schedule job from api call description.
studio-cli schedule apicall update [flags]
Options
-c, --cron string          Provide cron job configuration
-d, --data string          Data for a POST or PUT type call
-s, --description string   Brief description of the scheduled job
-e, --endpoint string      end point (e.g. /schedule/create)
-h, --help                 help for update
-n, --name string          Scheduled Job Name
-p, --params strings       Call parameters
-t, --type                 http request type, [DELETE|GET|HEAD|POST|PUT]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 382 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli schedule apicall  - ApiCall commands
studio-cli scm
Source Code Management Commands
Synopsis
The following commands are intended for interacting with Source Code Management tools.
The glab tool is the golang gitlab cli tool, where a wrapper is provided which handles all the
authentication.
The gitlab tool is a wrapper for the python v ersion of gitlab which is run with “python3 -m gitlab”. The
optional tool can be installed with by running: pip3 install –upgrade python-gitlab This integration sets
the GITLAB_URL and GITLAB_PRIV ATE_TOKEN automatically unless you set them manually in the
environment.
Options
-h, --help   help for scm
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 383 -->

## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli scm access-token  - Scm access-token command group
studio-cli scm branch  - Scm gitlab branch command group.
studio-cli scm gitlab  - Wrapper for python-gitlab command (requires python-gitlab to be installed)
studio-cli scm glab  - Wrapper for glab command, requires glab is installed with studio-cli
studio-cli scm group  - Scm gitlab group command group.
studio-cli scm project  - Scm gitlab project command group.
studio-cli scm ssh-key  - Scm ssh-key command group
studio-cli scm access-token
Scm access-token command group
Synopsis
Scm access-token command group.
Options
-h, --help   help for access-token
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm  - Source Code Management Commands
studio-cli scm access-token add  - Add an access token to your gitlab proﬁle
studio-cli scm access-token list  - List your access tokens on ﬁle in gitlab
studio-cli scm access-token remov e - Remov e an access token from your gitlab proﬁle

<!-- Page 384 -->

studio-cli scm access-token add
Add an access token to your gitlab proﬁle
Synopsis
Add an access token to your gitlab proﬁle
studio-cli scm access-token add [flags]
Options
-e, --expires-at string   Expire token string (format: "Jul 26, 2022"
-h, --help                help for add
-n, --name string         Name of token
-s, --scopes string       token scope default(api,write_repository)
[api,read_user,read_api,read_repository,write_repository,read_registry,write_registry,sudo]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm access-token  - Scm access-token command group
studio-cli scm access-token list
List your access tokens on ﬁle in gitlab
Synopsis
List your access tokens on ﬁle in gitlab
studio-cli scm access-token list [flags]
Options
-h, --help        help for list
-q, --jq string   jq query string override

<!-- Page 385 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm access-token  - Scm access-token command group
studio-cli scm access-token remove
Remov e an access token from your gitlab proﬁle
Synopsis
Remov e an access token from your gitlab proﬁle.
studio-cli scm access-token remove [flags]
Options
-h, --help        help for remove
-i, --id string    Access token key id to remove from gitlab profile (id from tokenlist)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm access-token  - Scm access-token command group

<!-- Page 386 -->

studio-cli scm branch
Scm gitlab branch command group.
Synopsis
Scm gitlab branch command group.
Options
-h, --help        help for branch
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm  - Source Code Management Commands
studio-cli scm branch create  - For gitlab branch create.
studio-cli scm branch delete  - Delete resource
studio-cli scm branch get  - Get resource
studio-cli scm branch search  - Search resource
studio-cli scm branch update  - For gitlab project update access group users.
studio-cli scm branch create
For gitlab branch create.
Synopsis
For gitlab branch create.
studio-cli scm branch create [flags]

<!-- Page 387 -->

Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags":"{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
--project-ID string       project ID
-r, --ref string              select source branch
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is

<!-- Page 388 -->

true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm branch  - Scm gitlab branch command group.
studio-cli scm branch delete
Delete resource
Synopsis
Delete resource
studio-cli scm branch delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode

<!-- Page 389 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm branch  - Scm gitlab branch command group.
studio-cli scm branch get
Get resource
Synopsis
Get resource
studio-cli scm branch get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm branch  - Scm gitlab branch command group.
studio-cli scm branch search
Search resource

<!-- Page 390 -->

Synopsis
Search resource
studio-cli scm branch search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "branch")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first

<!-- Page 391 -->

time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm branch  - Scm gitlab branch command group.
studio-cli scm branch update
For gitlab project update access group users.
Synopsis
For gitlab project update access group users.
studio-cli scm branch update [flags]
Options
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change

<!-- Page 392 -->

-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm branch  - Scm gitlab branch command group.
studio-cli scm gitlab
Wrapper for python-gitlab command (requires python-gitlab to be installed)
Synopsis
Wrapper for python-gitlab command (requires python-gitlab to be installed).
studio-cli scm gitlab [flags]
Options
-h, --help   help for gitlab

<!-- Page 393 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm  - Source Code Management Commands
studio-cli scm glab
Wrapper for glab command, requires glab is installed with studio-cli
Synopsis
Wrapper for glab command, requires glab is installed with studio-cli
studio-cli scm glab [flags]
Options
-h, --help   help for glab
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm  - Source Code Management Commands

<!-- Page 394 -->

studio-cli scm group
Scm gitlab group command group.
Synopsis
Scm gitlab group command group.
Options
-h, --help        help for group
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm  - Source Code Management Commands
studio-cli scm group assign  - Assign group access
studio-cli scm group create  - For gitlab group create.
studio-cli scm group delete  - Delete resource
studio-cli scm group get  - Get resource
studio-cli scm group revoke  - Revoke group access
studio-cli scm group search  - Search resource
studio-cli scm group update  - For gitlab project update access group users.
studio-cli scm group assign
Assign group access
Synopsis
Assign a group access
studio-cli scm group assign [flags]

<!-- Page 395 -->

Options
-g, --group string      group name.
-h, --help              help for assign
-n, --name string       Resource name
-r, --rolename string   Role (viewer, tester, editor, or lead) (default "viewer")
-w, --wrrn string       wrrn of resource.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm group  - Scm gitlab group command group.
studio-cli scm group create
For gitlab group create.
Synopsis
For gitlab group create.
studio-cli scm group create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags":"{\"key\":\"value\"}",
"url":"",
"tool-id":""
}

<!-- Page 396 -->

-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-id string         Parent group ID
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
--visibility string       Visibility for group, public|internal|private (default
"private")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode

<!-- Page 397 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm group  - Scm gitlab group command group.
studio-cli scm group delete
Delete resource
Synopsis
Delete resource
studio-cli scm group delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm group  - Scm gitlab group command group.
studio-cli scm group get
Get resource

<!-- Page 398 -->

Synopsis
Get resource
studio-cli scm group get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm group  - Scm gitlab group command group.
studio-cli scm group revoke
Revoke group access
Synopsis
Revoke a group access
studio-cli scm group revoke [flags]
Options
-g, --group string   group name.
-h, --help           help for revoke
-n, --name string    Resource name
-w, --wrrn string    wrrn of resource.

<!-- Page 399 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm group  - Scm gitlab group command group.
studio-cli scm group search
Search resource
Synopsis
Search resource
studio-cli scm group search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm

<!-- Page 400 -->

- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "group")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm group  - Scm gitlab group command group.

<!-- Page 401 -->

studio-cli scm group update
For gitlab project update access group users.
Synopsis
For gitlab project update access group users.
studio-cli scm group update [flags]
Options
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change
-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 402 -->

## See Also
studio-cli scm group  - Scm gitlab group command group.
studio-cli scm project
Scm gitlab project command group.
Synopsis
Scm gitlab project command group.
Options
-h, --help        help for project
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm  - Source Code Management Commands
studio-cli scm project create  - For gitlab project create.
studio-cli scm project delete  - Delete resource
studio-cli scm project get  - Get resource
studio-cli scm project search  - Search resource
studio-cli scm project update  - For gitlab project update access group users.
studio-cli scm project create
For gitlab project create.

<!-- Page 403 -->

Synopsis
For gitlab project create.
studio-cli scm project create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags":"{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-id string         Parent group ID
--group-name string       user group name
-h, --help                    help for create
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")

<!-- Page 404 -->

-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
--visibility string       Visibility for project, public|internal|private (default
"private")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm project  - Scm gitlab project command group.
studio-cli scm project delete
Delete resource
Synopsis
Delete resource
studio-cli scm project delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn

<!-- Page 405 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm project  - Scm gitlab project command group.
studio-cli scm project get
Get resource
Synopsis
Get resource
studio-cli scm project get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm project  - Scm gitlab project command group.

<!-- Page 406 -->

studio-cli scm project search
Search resource
Synopsis
Search resource
studio-cli scm project search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "repo")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state

<!-- Page 407 -->

- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm project  - Scm gitlab project command group.
studio-cli scm project update
For gitlab project update access group users.
Synopsis
For gitlab project update access group users.
studio-cli scm project update [flags]
Options
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm

<!-- Page 408 -->

true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change
-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm project  - Scm gitlab project command group.
studio-cli scm ssh-key
Scm ssh-key command group
Synopsis
Scm ssh-key command group.
Options
-h, --help   help for ssh-key

<!-- Page 409 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm  - Source Code Management Commands
studio-cli scm ssh-key add  - Add an ssh key to your gitlab proﬁle
studio-cli scm ssh-key list  - List your ssh keys on ﬁle in gitlab
studio-cli scm ssh-key remov e - Remov e an ssh key from your gitlab proﬁle
studio-cli scm ssh-key add
Add an ssh key to your gitlab proﬁle
Synopsis
Add an ssh key to your gitlab proﬁle
studio-cli scm ssh-key add [flags]
Options
-h, --help         help for add
-k, --key string   ssh key file to add to gitlab
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 410 -->

## See Also
studio-cli scm ssh-key  - Scm ssh-key command group
studio-cli scm ssh-key list
List your ssh keys on ﬁle in gitlab
Synopsis
List your ssh keys on ﬁle in gitlab
studio-cli scm ssh-key list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm ssh-key  - Scm ssh-key command group
studio-cli scm ssh-key remove
Remov e an ssh key from your gitlab proﬁle
Synopsis
Remov e an ssh key from your gitlab proﬁle
studio-cli scm ssh-key remove [flags]

<!-- Page 411 -->

Options
-h, --help        help for remove
-i, --id string   ssh key id to remove from gitlab profile (id from sshlist)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli scm ssh-key  - Scm ssh-key command group
studio-cli secure
Security related commands
Synopsis
The following commands are for managing secrets or connecting to the REST APIs to manage secrets.
The v ault command is a wrapper for the oﬃcial v ault cli which sets the environment v ariables
VAUL T_ADDR and V AUL T_TOKEN automatically based on what ev er WR Studio deployment you
are accessing. If you set either V AUL T_ADDR or V AUL T_TOKEN in the environment prior to running
studio-cli those v alues will be used instead.
Options
-h, --help   help for secure
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 412 -->

## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli secure group  - Group commands
studio-cli secure http  - Curl sty le access with dynamic tokens to the v ault API
studio-cli secure secret  - Secure secret command group
studio-cli secure token  - Secure token command group
studio-cli secure v ault - Vault wrapper command, requires v ault installed with studio-cli
studio-cli secure group
Group commands
Synopsis
Run a secure group sub command
Options
-h, --help   help for group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure  - Security related commands
studio-cli secure group assign  - Assign group access
studio-cli secure group revoke  - Revoke group access
studio-cli secure group assign
Assign group access

<!-- Page 413 -->

Synopsis
Assign a group access
studio-cli secure group assign [flags]
Options
-g, --group string      group name.
-h, --help              help for assign
-n, --name string       Resource name
-r, --rolename string   Role (viewer, tester, editor, or lead) (default "viewer")
-w, --wrrn string       wrrn of resource.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure group  - Group commands
studio-cli secure group revoke
Revoke group access
Synopsis
Revoke a group access
studio-cli secure group revoke [flags]
Options
-g, --group string   group name.
-h, --help           help for revoke
-n, --name string    Resource name
-w, --wrrn string    wrrn of resource.

<!-- Page 414 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure group  - Group commands
studio-cli secure http
Curl sty le access with dynamic tokens to the v ault API
Synopsis
The v ault HTTP API documentation can be found here: https://www.v aultproject.io/api-docs
With the http command, there is no need to acquire a security token, as it is taken care of automatically.
There is also no need to add the token authorization header. If you would prefer to use curl directly
you can use the getv aulttoken command to obtain a v ault token.
You can specify the optional data via the –data or –bodyﬁle argument if it is a POST or PUT call. If the
ﬁrst character of the -d/–data argument is an @ character, what follows will be used as a ﬁlename.
You can replace a curl command as follows: curl
–header “X-V ault-Token: …”
http://127.0.0.1:8200/v1/auth/token/lookup-self W ith:
studio-cli secure http -e /v1/auth/token/lookup-self
studio-cli secure http [flags]
Options
-b, --bodyfile string   Data from file for a POST or PUT type call
-d, --data string       Data for a POST or PUT type call
-e, --endpoint string   end point (e.g. /pipeline/list)
-H, --header strings    Pass a custom header(s)
-h, --help              help for http
-q, --jq string         jq query string override
-X, --request           http request type, [DELETE|GET|HEAD|LIST|POST|PUT] (default GET)

<!-- Page 415 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure  - Security related commands
studio-cli secure secret
Secure secret command group
Synopsis
Secure secret command group.
Options
-h, --help        help for secret
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 416 -->

## See Also
studio-cli secure  - Security related commands
studio-cli secure secret create  - Create resource
studio-cli secure secret delete  - Delete resource
studio-cli secure secret get  - Get resource
studio-cli secure secret search  - Search resource
studio-cli secure secret update  - Update resource
studio-cli secure secret create
Create resource
Synopsis
Create resource
studio-cli secure secret create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",
"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags":"{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-name string       user group name
-h, --help                    help for create
--kv-pair string          kv-pair like "key=value" (Use comma for more than one:
"key1=value1,key2=value2")
--kv-path string          kv-path like "kv/pathtest"
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command

<!-- Page 417 -->

false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure secret  - Secure secret command group

<!-- Page 418 -->

studio-cli secure secret delete
Delete resource
Synopsis
Delete resource
studio-cli secure secret delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure secret  - Secure secret command group
studio-cli secure secret get
Get resource
Synopsis
Get resource
studio-cli secure secret get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string

<!-- Page 419 -->

-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure secret  - Secure secret command group
studio-cli secure secret search
Search resource
Synopsis
Search resource
studio-cli secure secret search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:

<!-- Page 420 -->

- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "secret")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure secret  - Secure secret command group

<!-- Page 421 -->

studio-cli secure secret update
Update resource
Synopsis
Updates the v ault secret; if the kv-path is updated a new secret is generated while retaining the old one
studio-cli secure secret update [flags]
Options
-h, --help                 help for update
--kv-pair string       kv-pair like "key=value" (Use comma for more than one:
"key1=value1,key2=value2")
--kv-path string       kv-path like "kv/pathtest"
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change
-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 422 -->

## See Also
studio-cli secure secret  - Secure secret command group
studio-cli secure token
Secure token command group
Synopsis
Secure token command group.
studio-cli secure token [flags]
Options
-h, --help   help for token
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure  - Security related commands
studio-cli secure token get  - Get a v ault service token
studio-cli secure token get
Get a v ault service token
Synopsis
This command returns either a new or cached v ault service token which can be used with curl or any
other kind of v ault operation. It uses your WR Studio credentials to obtain the token.
studio-cli secure token get [flags]

<!-- Page 423 -->

Options
-f, --force   force obtain a new token
-h, --help    help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli secure token  - Secure token command group
studio-cli secure vault
Vault wrapper command, requires v ault installed with studio-cli
Synopsis
Vault wrapper command, requires v ault installed with studio-cli
studio-cli secure vault [flags]
Options
-h, --help   help for vault
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 424 -->

## See Also
studio-cli secure  - Security related commands
studio-cli sysreg
System registry conﬁguration commands
Synopsis
System registry conﬁguration commands
Options
-h, --help   help for sysreg
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli sysreg group  - Group commands
studio-cli sysreg http  - Submit calls directly to the REST API
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg token  - Token commands
studio-cli sysreg user  - User commands
studio-cli sysreg group
Group commands
Synopsis
Run a registry group sub command

<!-- Page 425 -->

Options
-h, --help   help for group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg  - System registry conﬁguration commands
studio-cli sysreg group assign  - Assign group access
studio-cli sysreg group revoke  - Revoke group access
studio-cli sysreg group assign
Assign group access
Synopsis
Assign a group access
studio-cli sysreg group assign [flags]
Options
-g, --group string      group name.
-h, --help              help for assign
-n, --name string       Resource name
-r, --rolename string   Role (viewer, tester, editor, or lead) (default "viewer")
-w, --wrrn string       wrrn of resource.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode

<!-- Page 426 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg group  - Group commands
studio-cli sysreg group revoke
Revoke group access
Synopsis
Revoke a group access
studio-cli sysreg group revoke [flags]
Options
-g, --group string   group name.
-h, --help           help for revoke
-n, --name string    Resource name
-w, --wrrn string    wrrn of resource.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg group  - Group commands
studio-cli sysreg http
Submit calls directly to the REST API

<!-- Page 427 -->

Synopsis
The http subcommand works like a curl command where you specify the type of request and data via
the –data or –bodyﬁle argument if it is a POST or PUT call.
If the ﬁrst character of the -d/–data argument is an @ character, what follows will be used as a ﬁlename.
studio-cli sysreg http [flags]
Options
-b, --bodyfile string   Data from file for a POST or PUT type call
-d, --data string       Data for a POST or PUT type call
-e, --endpoint string   end point (e.g. /users/current)
-h, --help              help for http
-q, --jq string         jq query string
-t, --type              http request type, [DELETE|GET|HEAD|POST|PUT]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg  - System registry conﬁguration commands
studio-cli sysreg project
Project manipulation commands
Synopsis
Project manipulation commands
Options
-h, --help        help for project
-q, --jq string   jq query string

<!-- Page 428 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg  - System registry conﬁguration commands
studio-cli sysreg project create  - Create a container registry project
studio-cli sysreg project delete  - Delete resource
studio-cli sysreg project get  - Get resource
studio-cli sysreg project group  - Group commands
studio-cli sysreg project list  - List container registry projects
studio-cli sysreg project members  - Members commands
studio-cli sysreg project repo  - Repo commands
studio-cli sysreg project robot  - Robot commands
studio-cli sysreg project search  - Search resource
studio-cli sysreg project update  - Update a container registry project
studio-cli sysreg project user  - User commands
studio-cli sysreg project create
Create a container registry project
Synopsis
Create a container registry project
studio-cli sysreg project create [flags]
Options
--args-file string        A json file to specify arguments.
An example of json file:
{
"name":"",
"state":"",

<!-- Page 429 -->

"component-wrrn":"",
"category":"",
"manual":"",
"description":"",
"tags": "{\"key\":\"value\"}",
"url":"",
"tool-id":""
}
-c, --category string         (*) Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component getCategories
-w, --component-wrrn string   (*) The Studio generated wrrn for the component. obtained
using studio-cli rbac component search
-d, --description string      Enclosed in " if the string includes spaces
--group-name string       user group name
-h, --help                    help for create
-q, --jq string               jq query string
-m, --manual                  default = false if not included in command/API. If
included, the set to true by adding -m in command
false = [default] cli will send command to the
component to create the physical resource
in addition to crating a virtual resource
in the rsm
true = The CLI will only create the virtual
resource in resource manager.
this is used when managing the resources
outside of Studio where the end user will create the resource and change
-n, --name string             (*) Enclosed in " if the string includes spaces, naming
must be unique
--private                 Make the project private when creating
-s, --state string            (*) Current state of the  permitted value
[pending|ready|archive|deleted], use studio-cli resource getState
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database (default "ready")
-g, --tags string             Initial array of tags for the resource, json object of
key:value pairs. It can be empty and assign the tags later.
- single object: "{\"key\":\"value\"}"
- multiple objects: ["{\"key1\":\"value1\"}", "
{\"key2\":\"value2\"}"]
--tool-id string          (*) ToolId of the resource, used only when manual flag is
true
-k, --unique-data string      Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-u, --url string              (*) Url of the resource, used only when manual flag is
true, include https:// for the use by the UI when its available

<!-- Page 430 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project delete
Delete resource
Synopsis
Delete resource
studio-cli sysreg project delete [flags]
Options
-f, --force         force delete(logical only)
-h, --help          help for delete
-m, --manual        manual
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands

<!-- Page 431 -->

studio-cli sysreg project get
Get resource
Synopsis
Get resource
studio-cli sysreg project get [flags]
Options
-h, --help          help for get
-q, --jq string     jq query string
-n, --name string   Resource name
-w, --wrrn string   wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project group
Group commands
Synopsis
Group commands
Options
-h, --help   help for group

<!-- Page 432 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project group add  - Add group to project
studio-cli sysreg project group delete  - Delete group from container registry project
studio-cli sysreg project group update  - Update group role in project
studio-cli sysreg project group add
Add group to project
Synopsis
Add an additional group to the project with a role.
The role mappings are [WR Studio role == Harbor Role (RoleId # in Harbor)]:
viewer == Guest (3)
tester == Developer (2)
editor == Maintainer (4)
lead   == Project Admin (1)
studio-cli sysreg project group add [flags]
Options
-g, --group string   group name to add
-h, --help           help for add
-n, --name string    project name or project ID
-r, --role           Role to add to group [viewer|tester|editor|lead]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 433 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project group  - Group commands
studio-cli sysreg project group delete
Delete group from container registry project
Synopsis
Delete a group by group name or the group ID for the speciﬁed container project.
studio-cli sysreg project group delete [flags]
Options
-g, --group string   The group name or group ID to remove from project
-h, --help           help for delete
-n, --name string    the project name or ID
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project group  - Group commands

<!-- Page 434 -->

studio-cli sysreg project group update
Update group role in project
Synopsis
Update a group’s role in the speciﬁed project.
The role mappings are [WR Studio role == Harbor Role (RoleId # in Harbor )]: view er == Guest (3) tester
== Dev eloper (2) editor == Maintainer (4) lead == Project Admin (1)
studio-cli sysreg project group update [flags]
Options
-g, --group string   group name to modify
-h, --help           help for update
-n, --name string    project name or project ID
-r, --role           Role to add to group [viewer|tester|editor|lead]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project group  - Group commands
studio-cli sysreg project list
List container registry projects
Synopsis
List container registry projects
studio-cli sysreg project list [flags]

<!-- Page 435 -->

Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project members
Members commands
Synopsis
Members commands.
Options
-h, --help   help for members
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 436 -->

## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project members list  - List container registry project members
studio-cli sysreg project members list
List container registry project members
Synopsis
List container registry project members
studio-cli sysreg project members list [flags]
Options
-a, --all            display all or not,default: false
-h, --help           help for list
-n, --name string    the project name or ID
--page int       the page number (default 1)
--pagesize int   the page size (default 10)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project members  - Members commands
studio-cli sysreg project repo
Repo commands

<!-- Page 437 -->

Synopsis
Repo commands.
Options
-h, --help   help for repo
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project repo artifacts  - List artifacts in container repo
studio-cli sysreg project repo info  - Display container repository information
studio-cli sysreg project repo list  - List repositories in project
studio-cli sysreg project repo remov e - Remov e container repository from project
studio-cli sysreg project repo scan  - Initiate a repository scan on a speciﬁc tag or reference
studio-cli sysreg project repo tag  - Tag an artifact in a container repo
studio-cli sysreg project repo artifacts
List artifacts in container repo
Synopsis
List artifacts in container repo.
To query for tags you can use: –jq ‘[.[]|select(.tags)|{“digest”:.digest,”tags”:[.tags[].name]}]’ To query
for vunlerability report URL –jq ‘.
[]|select(.tags)|select(.tags[]|.name==”latest”)|.addition_links.vulnerabilities.href’ –raw Th result of the
query abov e can be used with the “http -t GET -e RESUL T”
studio-cli sysreg project repo artifacts [flags]

<!-- Page 438 -->

Options
-h, --help          help for artifacts
-n, --name string   Name of project
-r, --repo string   Name of repository
-t, --tag string    Search for artifact by tag name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project repo  - Repo commands
studio-cli sysreg project repo info
Display container repository information
Synopsis
Display container repository information
studio-cli sysreg project repo info [flags]
Options
-h, --help          help for info
-n, --name string   Name of project
-r, --repo string   Name of repository
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 439 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project repo  - Repo commands
studio-cli sysreg project repo list
List repositories in project
Synopsis
List repositories in project
studio-cli sysreg project repo list [flags]
Options
-h, --help          help for list
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project repo  - Repo commands
studio-cli sysreg project repo remove
Remov e container repository from project
Synopsis
Remov e container repository from project

<!-- Page 440 -->

studio-cli sysreg project repo remove [flags]
Options
-h, --help          help for remove
-n, --name string   Name of project
-r, --repo string   Name of repository
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project repo  - Repo commands
studio-cli sysreg project repo scan
Initiate a repository scan on a speciﬁc tag or reference
Synopsis
Initiate a repository scan on a speciﬁc tag or reference
studio-cli sysreg project repo scan [flags]
Options
-h, --help          help for scan
-n, --name string   Name of project
-r, --repo string   Name of repository
-t, --tag string    Name of repository
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 441 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project repo  - Repo commands
studio-cli sysreg project repo tag
Tag an artifact in a container repo
Synopsis
Add a tag, delete a tag or display tags for a giv en artifact in a container repo.
When run without a tag argument the tags are display ed. When run with a tag argument a tag is
added. If the delete argument speciﬁed along with a tag argument the tag will be remov ed from the
artifact.
The artifact hash can optionally be omitted if deleting a tag, which will cause a search for the artifact to
remov e the tag.
studio-cli sysreg project repo tag [flags]
Options
-a, --artifact string   Artifact digest hash (found from repoartifacts command)
-d, --delete            Delete specified tag
-h, --help              help for tag
-n, --name string       Name of project
-r, --repo string       Name of repository
-t, --tag strings       Artifact digest hash (found from repoartifacts command)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 442 -->

## See Also
studio-cli sysreg project repo  - Repo commands
studio-cli sysreg project robot
Robot commands
Synopsis
Robot commands.
Options
-h, --help   help for robot
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project robot create  - Create robot account
studio-cli sysreg project robot list  - List robot accounts
studio-cli sysreg project robot remov e - Remov e robot account
studio-cli sysreg project robot create
Create robot account
Synopsis
Create robot account
studio-cli sysreg project robot create [flags]

<!-- Page 443 -->

Options
-h, --help               help for create
-n, --name string        Name of project
-r, --robotname string   Name of robot
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project robot  - Robot commands
studio-cli sysreg project robot list
List robot accounts
Synopsis
List robot accounts
studio-cli sysreg project robot list [flags]
Options
-h, --help          help for list
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 444 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project robot  - Robot commands
studio-cli sysreg project robot remove
Remov e robot account
Synopsis
Remov e robot account
studio-cli sysreg project robot remove [flags]
Options
-h, --help               help for remove
-n, --name string        Name of project
-r, --robotname string   Name of robot
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project robot  - Robot commands
studio-cli sysreg project search
Search resource

<!-- Page 445 -->

Synopsis
Search resource
studio-cli sysreg project search [flags]
Options
-c, --category string         Categories used by Studio components to filter when
searching for components.
Use the command to find those in use: studio-cli rbac
component get-category
--component-wrrn string   The Studio generated wrrn for the component. obtained using
studio-cli rbac component search
-h, --help                    help for search
-q, --jq string               jq query string
-k, --keys string             keys
-l, --limit float32           The max number of items to return - greater than 0 (default
10)
-n, --name string             Resource name
-p, --page float32            Page number  - greater than 0 (default 1)
-t, --resource-type string    The type of component. This is critical for the CLI to
enable creating associated resource, the type is used to validate
supported types for studio-cli rbac resource
command to work:
- vault
- gitlab
- artifacts
other types to used and will be supported:
- 3P tools [blackduck|coverity]
- rsm
- um
- ntf
- vlab
- tozny
- platformhealth
- pcy
- ota
- sysreg
- devreg
- conductor
- lxbs
- vxbs
- taf
- dfl
- plm
- mcp
- system
(default "project")
-s, --state string            Current state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under
creation, upgrade, etc
- ready: available for use

<!-- Page 446 -->

- deleted: when this object is deleted the first
time, its state is changed to deleted
and it won't show up in a CLI search but
will show up in API search.
Second time its deleted it is deleted
from  database
--tool-id string          tool id of resource
-u, --url string              Url of the resource, used only when manual flag is true,
include https:// for the use by the UI when its available
-v, --values string           values
-w, --wrrn string             Resource unique identifier - WRRN
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project update
Update a container registry project
Synopsis
Update a container registry project
studio-cli sysreg project update [flags]
Options
-h, --help                 help for update
-m, --manual               default = false if not included in command/API. If included,
the set to true by adding -m in command
false = [default] cli will send command to the component to
create the rbac resource
in addition to crating a virtual resource in the
rsm
true = The CLI will only create the virtual resource in
resource manager.
this is used when managing the resources outside of
Studio where the end user will create the resource and change

<!-- Page 447 -->

-s, --state string         New state of the resource. permitted value
[pending|ready|archive|deleted], use studio-cli resource get-state
- pending: future use when the location is under creation,
upgrade, etc
- ready: available for use
- deleted: when this object is deleted the first time, its
state is changed to deleted
and it won't show up in a CLI search but will
show up in API search.
Second time its deleted it is deleted from
database
-k, --unique-data string   Json object with internal " escaped, example: "
{\"key\":\"value\"}". Specified as string or @file.
-w, --wrrn string          wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project user
User commands
Synopsis
User commands
Options
-h, --help   help for user
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 448 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project  - Project manipulation commands
studio-cli sysreg project user add  - Add user to project
studio-cli sysreg project user delete  - Delete user from container registry project
studio-cli sysreg project user update  - Update user role in project
studio-cli sysreg project user add
Add user to project
Synopsis
Add an additional user to the project with a role.
The role mappings are [WR Studio role == Harbor Role (RoleId # in Harbor )]: view er == Guest (3) tester
== Dev eloper (2) editor == Maintainer (4) lead == Project Admin (1)
studio-cli sysreg project user add [flags]
Options
-h, --help          help for add
-n, --name string   project name or project ID
-r, --role          Role to add to group [viewer|tester|editor|lead]
-u, --user string   user name to add
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 449 -->

## See Also
studio-cli sysreg project user  - User commands
studio-cli sysreg project user delete
Delete user from container registry project
Synopsis
Delete a user by user name or the member ID for the speciﬁed container project.
studio-cli sysreg project user delete [flags]
Options
-h, --help          help for delete
-n, --name string   the project name or ID
-u, --user string   The user name or member ID to remove from project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project user  - User commands
studio-cli sysreg project user update
Update user role in project
Synopsis
Update a user ’s role in the speciﬁed project.
The role mappings are [WR Studio role == Harbor Role (RoleId # in Harbor )]: view er == Guest (3) tester
== Dev eloper (2) editor == Maintainer (4) lead == Project Admin (1)

<!-- Page 450 -->

studio-cli sysreg project user update [flags]
Options
-h, --help          help for update
-n, --name string   project name or project ID
-r, --role          Role to add to group [viewer|tester|editor|lead]
-u, --user string   user name to modify
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg project user  - User commands
studio-cli sysreg token
Token commands
Synopsis
Token commands
Options
-h, --help        help for token
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 451 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg  - System registry conﬁguration commands
studio-cli sysreg token get  - Get cli access token (same as User Proﬁle in the W eb UI)
studio-cli sysreg token get
Get cli access token (same as User Proﬁle in the W eb UI)
Synopsis
Get cli access token (same as User Proﬁle in the W eb UI)
studio-cli sysreg token get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg token  - Token commands
studio-cli sysreg user
User commands
Synopsis
User commands

<!-- Page 452 -->

Options
-h, --help        help for user
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg  - System registry conﬁguration commands
studio-cli sysreg user assign-admin  - Assign user admin role
studio-cli sysreg user info  - List container registry current user account details
studio-cli sysreg user list  - List container registry users
studio-cli sysreg user unassign-admin  - Remov e user admin role
studio-cli sysreg user assign-admin
Assign user admin role
Synopsis
Add the harbor administrator role to a user by name or ID.
studio-cli sysreg user assign-admin [flags]
Options
-h, --help          help for assign-admin
-n, --name string   user name or ID
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 453 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg user  - User commands
studio-cli sysreg user info
List container registry current user account details
Synopsis
List container registry current user account details
studio-cli sysreg user info [flags]
Options
-h, --help   help for info
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg user  - User commands
studio-cli sysreg user list
List container registry users

<!-- Page 454 -->

Synopsis
List container registry users
studio-cli sysreg user list [flags]
Options
-a, --all            display all or not,default: false
-h, --help           help for list
--page int       the page number (default 1)
--pagesize int   the page size (default 10)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg user  - User commands
studio-cli sysreg user unassign-admin
Remov e user admin role
Synopsis
Remov e the harbor administrator role from a user by name or ID.
studio-cli sysreg user unassign-admin [flags]
Options
-h, --help          help for unassign-admin
-n, --name string   user name or ID

<!-- Page 455 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli sysreg user  - User commands
studio-cli taf
Test Automation Framework Commands
Synopsis
Test Automation Framework Commands
Options
-h, --help        help for taf
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 456 -->

## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli taf agent  - Agent commands
studio-cli taf execution  - Execution commands
studio-cli taf generate  - Generate commands
studio-cli taf group  - Group commands
studio-cli taf health-check  - For T AF internal use to check dependencies status
studio-cli taf logs  - Logs commands
studio-cli taf plan  - Plan commands
studio-cli taf plugin  - Plugin commands
studio-cli taf project  - Project commands
studio-cli taf suite  - Test suite commands
studio-cli taf target  - Target commands
studio-cli taf agent
Agent commands
Synopsis
Agent commands
Options
-h, --help   help for agent
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 457 -->

## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf agent add  - Add agent by project id
studio-cli taf agent delete  - Delete agent by id
studio-cli taf agent get  - Get agent by id
studio-cli taf agent list  - List agents by project id
studio-cli taf agent add
Add agent by project id
Synopsis
Add agent by project id
studio-cli taf agent add [flags]
Options
-f, --config-file string   Provide Agent definition via a json file.
An example of json file:
{
"name": "TS04",
"projectId": "d063c011-d4a1-47b8-9692-9e01b5558fd4",
"agentManagementConfig": {
"targetId": "0e4730bc-66c7-47e2-8243-a5a88261aeae",
"targetName": "qemu-zynqmp-wrlinux"
},
"testCode": "minio/workspace-taf/artifacts-abc/c1cdb497-
3e1b-4729-9975-b2c7ffefddb3/abc",
"configurationFile": "/workspace-taf/artifacts-
abc/c1cdb497-3e1b-4729-9975-b2c7ffefddb3/testcode/pilot/setup.sh",
"secretPath": "optional",
"ramResourceName": "optional"
}
-h, --help                 help for add
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 458 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf agent  - Agent commands
studio-cli taf agent delete
Delete agent by id
Synopsis
Delete agent by id
studio-cli taf agent delete [flags]
Options
-a, --agent-id string     Agent id
-h, --help                help for delete
-p, --project-id string   Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf agent  - Agent commands
studio-cli taf agent get
Get agent by id

<!-- Page 459 -->

Synopsis
Get agent by id
studio-cli taf agent get [flags]
Options
-a, --agent-id string     Agent id
-h, --help                help for get
-p, --project-id string   Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf agent  - Agent commands
studio-cli taf agent list
List agents by project id
Synopsis
List agents by project id
studio-cli taf agent list [flags]
Options
-h, --help                help for list
-p, --project-id string   Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 460 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf agent  - Agent commands
studio-cli taf execution
Execution commands
Synopsis
Execution commands
Options
-h, --help   help for execution
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 461 -->

## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf execution cancel  - Cancel execution by id
studio-cli taf execution get  - Get execution by id
studio-cli taf execution get-ssh-details  - Get ssh details for execution id
studio-cli taf execution list  - List executions
studio-cli taf execution release-target  - Release target by execution id and test job id
studio-cli taf execution rerun  - Rerun execution
studio-cli taf execution result  - Get execution result by id
studio-cli taf execution run  - Executes and store the test plan execution
studio-cli taf execution cancel
Cancel execution by id
Synopsis
Cancel execution by id
studio-cli taf execution cancel [flags]
Options
-i, --execution-id string   Execution id
-h, --help                  help for cancel
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf execution  - Execution commands

<!-- Page 462 -->

studio-cli taf execution get
Get execution by id
Synopsis
Get execution by id
studio-cli taf execution get [flags]
Options
-i, --execution-id string   Execution id
-h, --help                  help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf execution  - Execution commands
studio-cli taf execution get-ssh-details
Get ssh details for execution id
Synopsis
Get ssh details for execution id
studio-cli taf execution get-ssh-details [flags]
Options
-i, --execution-id string   Execution id
-h, --help                  help for get-ssh-details

<!-- Page 463 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf execution  - Execution commands
studio-cli taf execution list
List executions
Synopsis
List executions
studio-cli taf execution list [flags]
Options
-h, --help                help for list
-l, --limit int           Specify the maximum number of rows to display (0 for no limit).
-p, --project-id string   Project id
-s, --status string       Execution status (active/completed)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf execution  - Execution commands

<!-- Page 464 -->

studio-cli taf execution release-target
Release target by execution id and test job id
Synopsis
Release target by execution id and test job id
studio-cli taf execution release-target [flags]
Options
-i, --execution-id string   Execution id
-h, --help                  help for release-target
-j, --test-job-id string    Test job id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf execution  - Execution commands
studio-cli taf execution rerun
Rerun execution
Synopsis
Rerun execution
studio-cli taf execution rerun [flags]

<!-- Page 465 -->

Options
-i, --execution-id string   Provide an execution id
-h, --help                  help for rerun
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf execution  - Execution commands
studio-cli taf execution result
Get execution result by id
Synopsis
Get execution result by id
studio-cli taf execution result [flags]
Options
-i, --execution-id string   Execution id
-h, --help                  help for result
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 466 -->

## See Also
studio-cli taf execution  - Execution commands
studio-cli taf execution run
Executes and store the test plan execution
Synopsis
Executes and store the test plan execution
studio-cli taf execution run [flags]
Options
-c, --command-line-file string   Command line file
-f, --config-file string         Provide execution definition via json file.
An example of json file:
{
"testSuitesEnvironments": [{
"id": "YourTestSuiteId",
"environments": [{
"type": 0,
"target": "Target Id",
"retention": "release/retain"
},
{
"type": 1,
"value": "Artifact Example",
"target": "Target ID",
"retention": "release/retain"
},
{
"type": 2,
"value": "SSH Example"
},
{
"type": 3,
"value": "SSH Example",
"agent": "Agent Id"
}]
}]
}
-h, --help                       help for run
-l, --log-level int              Provide logLevel (default 20)
-p, --test-plan string           Test plan id

<!-- Page 467 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf execution  - Execution commands
studio-cli taf generate
Generate commands
Synopsis
Generate commands
Options
-h, --help   help for generate
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf generate project  - Generate project

<!-- Page 468 -->

studio-cli taf generate project
Generate project
Synopsis
Generate project
studio-cli taf generate project [flags]
Options
-c, --code string          Project code
-d, --description string   Description
-g, --group-name string    Group name
-h, --help                 help for project
-n, --name string          Project name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf generate  - Generate commands
studio-cli taf group
Group commands
Synopsis
Group commands
Options
-h, --help   help for group

<!-- Page 469 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf group assign  - Assign a group of users access to a T AF project
studio-cli taf group revoke  - Revoke a group of users access to a T AF project
studio-cli taf group assign
Assign a group of users access to a T AF project
Synopsis
Assign a group of users access to a T AF project with a speciﬁc role
studio-cli taf group assign [flags]
Options
-i, --group-id string     Group id to assign the project
-g, --group-name string   Group name to assign the project
-h, --help                help for assign
-n, --name string         Project name
-r, --rbac-role string    Name for RBAC role, valid RBAC roles are
tester|viewer|editor|lead
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 470 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf group  - Group commands
studio-cli taf group revoke
Revoke a group of users access to a T AF project
Synopsis
Revoke a group of users access to a T AF project
studio-cli taf group revoke [flags]
Options
-i, --group-id string     Group id to assign the project
-g, --group-name string   Group name to assign the project
-h, --help                help for revoke
-n, --name string         Project name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf group  - Group commands
studio-cli taf health-check
For T AF internal use to check dependencies status

<!-- Page 471 -->

Synopsis
For T AF internal use to check dependencies status
studio-cli taf health-check [flags]
Options
-h, --help   help for health-check
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf logs
Logs commands
Synopsis
Logs commands
Options
-h, --help   help for logs
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 472 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf logs automation-test-logs  - Get automation test logs
studio-cli taf logs automation-test-logs
Get automation test logs
Synopsis
Get automation test logs
studio-cli taf logs automation-test-logs [flags]
Options
-e, --execution-id string   Specify the unique identifier of a test execution
-h, --help                  help for automation-test-logs
-j, --test-job-id string    Specify the unique identifier of a test job
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf logs  - Logs commands
studio-cli taf plan
Plan commands

<!-- Page 473 -->

Synopsis
Plan commands
Options
-h, --help   help for plan
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf plan archiv e - Archiv e plan
studio-cli taf plan clone  - Clone plan
studio-cli taf plan create  - Create plan
studio-cli taf plan get  - Get plan by id
studio-cli taf plan list  - List plan
studio-cli taf plan rename  - Rename plan by id
studio-cli taf plan update  - Update plan
studio-cli taf plan archive
Archiv e plan
Synopsis
Archiv e plan
studio-cli taf plan archive [flags]

<!-- Page 474 -->

Options
-h, --help                  help for archive
-p, --project-id string     Provide project id
-t, --test-plan-id string   Provide plan id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plan  - Plan commands
studio-cli taf plan clone
Clone plan
Synopsis
Clone plan
studio-cli taf plan clone [flags]
Options
-h, --help                   help for clone
-n, --new-plan-name string   New plan name / If not specified, the same name is set with
_CLONE suffix
-p, --project-id string      Project id of the plan you want to clone
-t, --test-plan-id string    Plan id that needs clone
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 475 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plan  - Plan commands
studio-cli taf plan create
Create plan
Synopsis
Create plan
studio-cli taf plan create [flags]
Options
-s, --auto-sync string                Automatically sync test plan when new test cases
are added to library: sync | noSync.
-d, --description string              Plan description
-f, --framework string                Framework name
-h, --help                            help for create
-n, --name string                     Plan name
-i, --plugin-id string                Plugin id
-p, --project-id string               Project id
-a, --reporting-plugins-ids strings   A list of active reporting plugins ids. For
example: -a "id1,id2"
-r, --target-retention string         Plan target retention (default | release | retain)
(default "default")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plan  - Plan commands

<!-- Page 476 -->

studio-cli taf plan get
Get plan by id
Synopsis
Get plan by id
studio-cli taf plan get [flags]
Options
-h, --help                  help for get
-p, --project-id string     Project id
-t, --test-plan-id string   Plan id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plan  - Plan commands
studio-cli taf plan list
List plan
Synopsis
List plan
studio-cli taf plan list [flags]
Options
-h, --help                help for list
-l, --limit int           Specify the maximum number of rows to display (0 for no limit).
-p, --project-id string   Project id

<!-- Page 477 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plan  - Plan commands
studio-cli taf plan rename
Rename plan by id
Synopsis
Rename plan by id
studio-cli taf plan rename [flags]
Options
-h, --help                  help for rename
-n, --name string           name
-p, --project-id string     Project id
-t, --test-plan-id string   Plan id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plan  - Plan commands

<!-- Page 478 -->

studio-cli taf plan update
Update plan
Synopsis
Update plan
studio-cli taf plan update [flags]
Options
-s, --auto-sync string                Automatically sync test plan when new test cases
are added to library: sync | noSync.
-d, --description string              Plan description
-f, --framework string                Framework name
-h, --help                            help for update
-n, --name string                     Plan name
-i, --plugin-id string                Plugin id
-p, --project-id string               Project id
-a, --reporting-plugins-ids strings   A list of active reporting plugins ids. For
example: -a "id1,id2"
-r, --target-retention string         Plan target retention (default | release | retain)
(default "default")
-t, --test-plan-id string             Plan id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plan  - Plan commands
studio-cli taf plugin
Plugin commands

<!-- Page 479 -->

Synopsis
Plugin command to install, uninstall and get plugins
Options
-h, --help   help for plugin
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf plugin arguments  - Add Command Line Arguments
studio-cli taf plugin install  - Install plugin command
studio-cli taf plugin list  - List plugins
studio-cli taf plugin status  - Plugin status
studio-cli taf plugin uninstall  - Uninstall plugin
studio-cli taf plugin arguments
Add Command Line Arguments
Synopsis
Add Command Line Arguments
studio-cli taf plugin arguments [flags]
Options
-f, --config-file string   Provide project definition via a json file.
An example of json file:
{
"commandLineArguments":

<!-- Page 480 -->

[
{
"type":"text",
"label": "example command line
arguments",
"name": "example",
"options": ["example options"],
"placeholder": "placeholder
example",
"defaultValue": "default text",
"order": 0,
"value": "value 1",
}
]
}
-h, --help                 help for arguments
-i, --plugin-id string     Plugin id
-p, --project-id string    Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plugin  - Plugin commands
studio-cli taf plugin install
Install plugin command
Synopsis
Install plugin command
studio-cli taf plugin install [flags]
Options
-A, --active              Flag to indicate the status of the plugin
-f, --file string         Plugin file .zip
-F, --force               Flag that determines whether the plugin installation requires

<!-- Page 481 -->

overriding
-h, --help                help for install
-p, --project-id string   Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plugin  - Plugin commands
studio-cli taf plugin list
List plugins
Synopsis
List plugins
studio-cli taf plugin list [flags]
Options
-f, --framework string    Framework to filter the list of returned plugins
-h, --help                help for list
-p, --project-id string   Project Id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 482 -->

## See Also
studio-cli taf plugin  - Plugin commands
studio-cli taf plugin status
Plugin status
Synopsis
Plugin status
studio-cli taf plugin status [flags]
Options
-A, --active              (False by default) Flag to indicate the new status of the
plugin
-h, --help                help for status
-o, --override            (False by default) Flag to indicate if the new plugin
activation would deactivate other conflicting plugins
-i, --plugin-id string    Plugin id
-p, --project-id string   Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plugin  - Plugin commands
studio-cli taf plugin uninstall
Uninstall plugin

<!-- Page 483 -->

Synopsis
Uninstall plugin
studio-cli taf plugin uninstall [flags]
Options
-c, --capability string   Plugin capability
-h, --help                help for uninstall
-n, --name string         Plugin name
-p, --project-id string   Project id
-v, --version string      Plugin version
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf plugin  - Plugin commands
studio-cli taf project
Project commands
Synopsis
Project commands
Options
-h, --help   help for project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses

<!-- Page 484 -->

--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf project create  - Create project from json
studio-cli taf project delete  - Delete project
studio-cli taf project get  - Get project
studio-cli taf project list  - List project
studio-cli taf project test-code-collection  - Test Code Collection commands
studio-cli taf project update  - Update project from json
studio-cli taf project create
Create project from json
Synopsis
Create project from json
studio-cli taf project create [flags]
Options
-f, --config-file string   Provide project definition via a json file.
An example of json file:
{
"projectCode": "VXWORKS",
"name": "VXWORKS",
"targetRetention": "release",
"description": "Project VxWorks",
"targetRetentionDuration": 8,
"rbacGroup": "TAF-Group-Name"
}
-h, --help                 help for create
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses

<!-- Page 485 -->

--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project  - Project commands
studio-cli taf project delete
Delete project
Synopsis
Delete project
studio-cli taf project delete [flags]
Options
--hard-delete         Enable hard delete of the project. By default is soft delete
-h, --help                help for delete
-i, --project-id string   Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project  - Project commands
studio-cli taf project get
Get project

<!-- Page 486 -->

Synopsis
Get project
studio-cli taf project get [flags]
Options
-h, --help                help for get
-p, --project-id string   Provide a project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project  - Project commands
studio-cli taf project list
List project
Synopsis
List project
studio-cli taf project list [flags]
Options
-c, --category string        Categories used by Studio components to filter when
searching for components.
-g, --get-from-taf string    Gets the data from TAF DB instead of RAM. By default is No
(gets the data from RAM to optimize the performance): Yes | No (default "No")
-h, --help                   help for list
-k, --keys string            Resource keys.
-l, --limit int              Specify the maximum number of rows to display (0 for no
limit).
-r, --resource-type string   The type of component.

<!-- Page 487 -->

-s, --state string           Current state of the resource. Permitted value:
[pending|ready|archive|deleted].
-t, --tool-id string         Tool id of the respective resource.
-v, --values string          Resource values.
-w, --wrrn string            Resource unique identifier - WRRN.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project  - Project commands
studio-cli taf project test-code-collection
Test Code Collection commands
Synopsis
Test Code Collection commands
Options
-h, --help   help for test-code-collection
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 488 -->

## See Also
studio-cli taf project  - Project commands
studio-cli taf project test-code-collection append  - Append test code collection from json
studio-cli taf project test-code-collection framework  - Get list of av ailable frameworks
studio-cli taf project test-code-collection list  - List test code collection
studio-cli taf project test-code-collection remov e - Remov e test code collection
studio-cli taf project test-code-collection test-case  - Test Case commands
studio-cli taf project test-code-collection upload  - Upload test code collection from json
studio-cli taf project test-code-collection append
Append test code collection from json
Synopsis
Append test code collection from json
studio-cli taf project test-code-collection append [flags]
Options
-f, --config-file string               Provide project definition via a json file.
An example of json file:
{
"config": {},
"framework": "pytest",
"testCollections": [{
"name": "VXWorks Regression",
"collectionType": "User Group",
"isDisabled": false,
"isAtomic": false,
"testCases": [],
"config": {},
"testCollections": [{
"name": "Collection A",
"collectionType":
"SOFTWARE_DEFINED_GROUP",
"isDisabled": false,
"isAtomic": false,
"testCases": [
{"name": "tc1"},
{"name": "tc2"}
],
"config": {
"layers":
["INCLUDE_TM_MEMMEM"]

<!-- Page 489 -->

},
"testCollections": []
}]
}]
}
-h, --help                             help for append
-i, --project-id string                Project id
-r, --replace                          Replace test code collection
-c, --test-code-collection-id string   Test Code Collection id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project test-code-collection  - Test Code Collection commands
studio-cli taf project test-code-collection framework
Get list of av ailable frameworks
Synopsis
Get list of av ailable frameworks for the project
studio-cli taf project test-code-collection framework [flags]
Options
-h, --help                help for framework
-p, --project-id string   Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string

<!-- Page 490 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project test-code-collection  - Test Code Collection commands
studio-cli taf project test-code-collection list
List test code collection
Synopsis
List test code collection
studio-cli taf project test-code-collection list [flags]
Options
-f, --framework string    Framework to filter the list of the returned test code
collections
-h, --help                help for list
-p, --project-id string   Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project test-code-collection  - Test Code Collection commands
studio-cli taf project test-code-collection remove
Remov e test code collection

<!-- Page 491 -->

Synopsis
Remov e test code collection
studio-cli taf project test-code-collection remove [flags]
Options
-h, --help                             help for remove
-p, --project-id string                Project id
-c, --test-code-collection-id string   Test code collection id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project test-code-collection  - Test Code Collection commands
studio-cli taf project test-code-collection test-case
Test Case commands
Synopsis
Test Case commands
Options
-h, --help   help for test-case
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string

<!-- Page 492 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project test-code-collection  - Test Code Collection commands
studio-cli taf project test-code-collection test-case get  - Get test case metadata
studio-cli taf project test-code-collection test-case get
Get test case metadata
Synopsis
Get test case metadata
studio-cli taf project test-code-collection test-case get [flags]
Options
-h, --help                             help for get
-n, --name string                      Test case name
-p, --project-id string                Project id
-t, --test-case-path string            Test case path
-c, --test-code-collection-id string   Test code collection id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project test-code-collection test-case  - Test Case commands

<!-- Page 493 -->

studio-cli taf project test-code-collection upload
Upload test code collection from json
Synopsis
Upload test code collection from json
studio-cli taf project test-code-collection upload [flags]
Options
-f, --config-file string   Provide project definition via a json file.
An example of json file:
{
"config": {},
"framework": "pytest",
"testCollections": [{
"name": "VXWorks Regression",
"collectionType": "User Group",
"isDisabled": false,
"isAtomic": false,
"testCases": [],
"config": {},
"testCollections": {
"name": "Collection A",
"collectionType": "SOFTWARE_DEFINED_GROUP",
"isDisabled": false,
"isAtomic": false,
"testCases": [
{"name": "tc1"},
{"name": "tc2"}
],
"config": {
"layers": ["INCLUDE_TM_MEMMEM"]
},
"testCollections": []
}
}]
}
-h, --help                 help for upload
--i string             Deprecated: Use '--project-id' or '-p' instead. (DEPRECATED:
This flag is deprecated. Use '--project-id' or '-p' instead.)
-p, --project-id string    Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 494 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project test-code-collection  - Test Code Collection commands
studio-cli taf project update
Update project from json
Synopsis
Update project from json
studio-cli taf project update [flags]
Options
-f, --config-file string   Provide project definition via a json file.
An example of json file:
{
"projectCode": "VXWORKS",
"name": "VXWORKS",
"description": "Project VxWorks",
"targetRetention": "release",
"targetRetentionDuration": 8,
"executionLogsRetention": "1",
"automationLogsRetention": "2"
}
-h, --help                 help for update
-p, --project-id string    Project id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf project  - Project commands

<!-- Page 495 -->

studio-cli taf suite
Test suite commands
Synopsis
Test suite commands
Options
-h, --help   help for suite
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf suite archiv e - Archiv e test suite by id
studio-cli taf suite create  - Create suite from json
studio-cli taf suite get  - Get test suite by id
studio-cli taf suite list  - List test suite
studio-cli taf suite update  - Update test suite from json
studio-cli taf suite archive
Archiv e test suite by id
Synopsis
Archiv e test suite by id
studio-cli taf suite archive [flags]

<!-- Page 496 -->

Options
-h, --help                   help for archive
-p, --project-id string      Project id
-i, --test-plan-id string    Test plan id
-t, --test-suite-id string   Test suite id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf suite  - Test suite commands
studio-cli taf suite create
Create suite from json
Synopsis
Create suite from json
studio-cli taf suite create [flags]
Options
-f, --config-file string    Provide test suite definition via a json file.
An example of json file:
{
"testPlanId": "Test plan id",
"name": "Test Suite 1",
"environments": [
{
"type": 0,
"target": "Target Id",
"retention": "release/retain"
},
{
"type": 1,
"value": "Artifact Example",

<!-- Page 497 -->

"target": "Target ID",
"retention": "release/retain"
},
{
"type": 2,
"value": "SSH Example"
}
],
"testSet": {
"testCases": [
{
"name": "memmemTest2",
"path": "VxWorks Regression/tmMemmem"
}
],
"testLibrary": {
"id": "aebe437a-4656-4603-b251-1c8afdbeee26",
"framework": "vxtest",
"config": {},
"projectId": "d89d8bb6-018b-4b89-a295-
fea00bc8ad13",
"testCollections": [
{
"name": "VxWorks Regression",
"collectionType": "USER_GROUP",
"isAtomic": true,
"testCases": [
{
"name": "memmemTest2",
"path": "VxWorks
Regression/tmMemmem",
"config": {
"layers": [

"INCLUDE_TM_OS_CORE_KERNEL_MULTICORE",

"INCLUDE_TM_OS_CORE_KERNEL_MEM"
]
},
"testCollections": [],
"isIndeterminate": true,
"isSelectable": true
}
]
}
],
"createdBy": "developer@windriver.com",
"createdDate": "2024-05-23T06:15:53.495Z",
"modifiedBy": "developer@windriver.com",
"modifiedDate": "2024-05-23T06:15:53.495Z"
}
}
}
-h, --help                  help for create
-p, --project-id string     Project id
-t, --test-plan-id string   Test plan id

<!-- Page 498 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf suite  - Test suite commands
studio-cli taf suite get
Get test suite by id
Synopsis
Get test suite by id
studio-cli taf suite get [flags]
Options
-h, --help                   help for get
-p, --project-id string      Project id
-i, --test-plan-id string    Test plan id
-t, --test-suite-id string   Test suite id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf suite  - Test suite commands

<!-- Page 499 -->

studio-cli taf suite list
List test suite
Synopsis
List test suite
studio-cli taf suite list [flags]
Options
-h, --help                  help for list
-p, --project-id string     Project id
-t, --test-plan-id string   Test plan id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf suite  - Test suite commands
studio-cli taf suite update
Update test suite from json
Synopsis
Update test suite from json
studio-cli taf suite update [flags]

<!-- Page 500 -->

Options
-f, --config-file string     Provide test suite definition via a json file.
An example of json file:
{
"testPlanId": "Test plan id",
"name": "Test Suite Name",
"id": "Test Suite ID",
"testPlanId": "Test Plan ID",
"environments": [
{
"type": 0,
"target": "Target Id",
"retention": "release/retain"
},
{
"type": 1,
"value": "Artifact Example",
"target": "Target ID",
"retention": "release/retain"
},
{
"type": 2,
"value": "SSH Example"
}
],
"testSet": {
"testCases": [
{
"name": "memmemTest2",
"path": "VxWorks Regression/tmMemmem"
}
],
"testCollections":[],
"testLibrary": {
"id": "aebe437a-4656-4603-b251-1c8afdbeee26",
"framework": "vxtest",
"config": {},
"projectId": "d89d8bb6-018b-4b89-a295-
fea00bc8ad13",
"testCollections": [
{
"name": "VxWorks Regression",
"collectionType": "USER_GROUP",
"testCases": [
{
"name": "memmemTest2",
"path": "VxWorks
Regression/tmMemmem",
"config": {
"layers": [

"INCLUDE_TM_OS_CORE_KERNEL_MULTICORE",

"INCLUDE_TM_OS_CORE_KERNEL_MEM"

<!-- Page 501 -->

]
},
"testCollections": [],
"isIndeterminate": true,
"isSelectable": true
}
]
}
],
"createdBy": "developer@windriver.com",
"createdDate": "2024-05-23T06:15:53.495Z",
"modifiedBy": "developer@windriver.com",
"modifiedDate": "2024-05-23T06:15:53.495Z"
}
}
}
-h, --help                   help for update
-i, --project-id string      Project id
-t, --test-suite-id string   Test suite id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf suite  - Test suite commands
studio-cli taf target
Target commands
Synopsis
Target commands
Options
-h, --help   help for target

<!-- Page 502 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf  - Test Automation Framework Commands
studio-cli taf target add  - Add target by project id
studio-cli taf target delete  - Delete target by target id
studio-cli taf target get  - Get target by target id
studio-cli taf target list  - List targets by project id
studio-cli taf target add
Add target by project id
Synopsis
Add target by project id
studio-cli taf target add [flags]
Options
-f, --config-file string   Provide target definition via a json file.
An example of json file:
{
"name": "TS03",
"projectId": "d063c011-d4a1-47b8-9692-9e01b5558fd4",
"targetManagementConfig": {
"targetId": "0e4730bc-66c7-47e2-8243-a5a88261aeae",
"targetName": "qemu-zynqmp-wrlinux"
},
"builderConfig": {},
"secretPath": "optional",
"ramResourceName": "optional"
}
-h, --help                 help for add

<!-- Page 503 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf target  - Target commands
studio-cli taf target delete
Delete target by target id
Synopsis
Delete target by target id
studio-cli taf target delete [flags]
Options
-h, --help                help for delete
-p, --project-id string   Project id
-t, --target-id string    Target id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf target  - Target commands

<!-- Page 504 -->

studio-cli taf target get
Get target by target id
Synopsis
Get target by target id
studio-cli taf target get [flags]
Options
-h, --help                help for get
-p, --project-id string   Project id
-t, --target-id string    Target id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf target  - Target commands
studio-cli taf target list
List targets by project id
Synopsis
List targets by project id
studio-cli taf target list [flags]
Options
-h, --help                help for list
-p, --project-id string   Project id

<!-- Page 505 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli taf target  - Target commands
studio-cli um
User management commands
Synopsis
User management commands.
Options
-h, --help   help for um
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 506 -->

## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli um adminhttp  - Submit calls directly to the Tozny Admin Console REST API
studio-cli um adminsettings  - Manage default realm token settings in the admin console
studio-cli um entitlement  - Entitlement manager
studio-cli um group  - Group management commands
studio-cli um permission  - Permission manager
studio-cli um role  - User management role commands
studio-cli um setting  - Setting manager
studio-cli um user  - Tozny user commands
studio-cli um adminhttp
Submit calls directly to the Tozny Admin Console REST API
Synopsis
The http subcommand works like a curl command where you specify the type of request and data via
the –data or –bodyﬁle argument if it is a POST or PUT call.
If the ﬁrst character of the -d/–data argument is an @ character, what follows will be used as a ﬁlename.
The adminhttp function is intended for adv anced debugging purposes only.
studio-cli um adminhttp [flags]
Options
-b, --bodyfile string   Data from file for a POST or PUT type call
-d, --data string       Data for a POST or PUT type call
-e, --endpoint string   end point (e.g. /auth/users)
-h, --help              help for adminhttp
-q, --jq string         jq query string override
-t, --type              http request type, [DELETE|GET|HEAD|POST|PUT]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 507 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um  - User management commands
studio-cli um adminsettings
Manage default realm token settings in the admin console
Synopsis
Get or set v ariables for the default realm in the which is found in the tozny admin console.
You can also add the environment v ariable USE_DASHBOARD_AUTH=1 to use the Tozny dashboard
login instead of a Tozny User token for admin console access.
USE_DASHBOARD_AUTH=1 studio-cli adminsettings –url
Please use extreme caution as this function should only be used by the installer to manage the
deployment.
studio-cli um adminsettings [flags]
Options
-h, --help          help for adminsettings
-q, --jq string     jq query string override
-n, --name string   Name of variable to get or set
-s, --set string    Set new value for named variable (default "___EMPTY___")
-u, --url           Print URL for admin console with token
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um  - User management commands

<!-- Page 508 -->

studio-cli um entitlement
Entitlement manager
Synopsis
Entitlement manager
Options
-h, --help        help for entitlement
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um  - User management commands
studio-cli um entitlement favorite  - Favorite application management commands
studio-cli um entitlement get  - Get user ’s entitlement.
studio-cli um entitlement list  - List all entitlements.
studio-cli um entitlement favorite
Favorite application management commands
Synopsis
Favorite application management commands
Options
-h, --help   help for favorite

<!-- Page 509 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um entitlement  - Entitlement manager
studio-cli um entitlement favorite add  - Add favorite application
studio-cli um entitlement favorite remov e - Remov e favorite application.
studio-cli um entitlement favorite add
Add favorite application
Synopsis
Add favorite application.
studio-cli um entitlement favorite add [flags]
Options
-h, --help        help for add
-i, --id string   id of the entitlement to be set as a favorite app for an user.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 510 -->

## See Also
studio-cli um entitlement favorite  - Favorite application management commands
studio-cli um entitlement favorite remove
Remov e favorite application.
Synopsis
Remov e favorite application.
studio-cli um entitlement favorite remove [flags]
Options
-h, --help        help for remove
-i, --id string   id of the entitlement to be removed as a favorite app for an user.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um entitlement favorite  - Favorite application management commands
studio-cli um entitlement get
Get user ’s entitlement.
Synopsis
Get user ’s entitlement.
studio-cli um entitlement get [flags]

<!-- Page 511 -->

Options
-h, --help        help for get
-i, --id string   ID of the user to get the entitlements. Use me (current user) if not
provided (default "me")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um entitlement  - Entitlement manager
studio-cli um entitlement list
List all entitlements.
Synopsis
List all entitlements.
studio-cli um entitlement list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 512 -->

## See Also
studio-cli um entitlement  - Entitlement manager
studio-cli um group
Group management commands
Synopsis
Group management commands.
Options
-h, --help        help for group
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 513 -->

## See Also
studio-cli um  - User management commands
studio-cli um group assign  - Assign a role to a group in tozny.
studio-cli um group attribute  - Display, add or remov e an attribute from a group.
studio-cli um group create  - Create a new group.
studio-cli um group delete  - Delete a group
studio-cli um group get  - Get data of a group by group name.
studio-cli um group list  - Get groups.
studio-cli um group read  - Read a group in tozny.
studio-cli um group resource  - Resource manager
studio-cli um group revoke  - Revoke a role from a group in tozny.
studio-cli um group search  - Search group
studio-cli um group update  - Update a group.
studio-cli um group user  - Group user management commands
studio-cli um group assign
Assign a role to a group in tozny.
Synopsis
Assign a role to a group in tozny.
studio-cli um group assign [flags]
Options
-h, --help              help for assign
-n, --name string       Group name
-r, --rolename string   Role name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 514 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group attribute
Display, add or remov e an attribute from a group.
Synopsis
Display add or remov e an attribute from a group. If giv en just the group name, all the attributes are
display ed. Using –delete + –key can remov e an attribute. Using –key + –v alue can set or update an
attribute.
This command requires the tozny console admin role.
studio-cli um group attribute [flags]
Options
-d, --delete         Delete key
-g, --group string   Group name
-h, --help           help for attribute
-k, --key string     Attribute key
-v, --value string   Attribute value
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands

<!-- Page 515 -->

studio-cli um group create
Create a new group.
Synopsis
Create a new group.
studio-cli um group create [flags]
Options
-d, --description string   Description for group
-h, --help                 help for create
-n, --name string          Name for group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group delete
Delete a group
Synopsis
Delete a group by its name.
studio-cli um group delete [flags]
Options
-h, --help          help for delete
-n, --name string   Name for group

<!-- Page 516 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group get
Get data of a group by group name.
Synopsis
Get data of a group by group name.
studio-cli um group get [flags]
Options
-h, --help          help for get
-i, --id string     ID for group
-n, --name string   Name for group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands

<!-- Page 517 -->

studio-cli um group list
Get groups.
Synopsis
Get groups.
studio-cli um group list [flags]
Options
-h, --help          help for list
-l, --limit int32   Limit of items to evaluate (default 5)
-p, --page int32    page of items to evaluate (default 1)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group read
Read a group in tozny.
Synopsis
Read a group in tozny.
studio-cli um group read [flags]
Options
-h, --help          help for read
-i, --id string     ID for group
-n, --name string   Group name

<!-- Page 518 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group resource
Resource manager
Synopsis
Resource manager
Options
-h, --help        help for resource
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group resource assign  - Assign resource to group.
studio-cli um group resource list  - Get all resources assigned to group.
studio-cli um group resource revoke  - Revoke a resource from group.

<!-- Page 519 -->

studio-cli um group resource assign
Assign resource to group.
Synopsis
Assign resource to group.
studio-cli um group resource assign [flags]
Options
-h, --help               help for assign
-i, --id string          Group id to assign the resource
-n, --name string        Group name to assign the resource
-r, --role-name string   Name for RBAC role, valid RBAC roles are
tester|viewer|editor|lead
-w, --wrrn string        Wind River resource number
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group resource  - Resource manager
studio-cli um group resource list
Get all resources assigned to group.
Synopsis
Get all resources assigned to group.
studio-cli um group resource list [flags]

<!-- Page 520 -->

Options
-h, --help          help for list
-i, --id string     Group id to query all resources
-n, --name string   Group name to query all resources
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group resource  - Resource manager
studio-cli um group resource revoke
Revoke a resource from group.
Synopsis
Revoke a resource from group.
studio-cli um group resource revoke [flags]
Options
-h, --help          help for revoke
-i, --id string     Group id to revoke the resource
-n, --name string   Group name to revoke the resource
-w, --wrrn string   Wind River resource number
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 521 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group resource  - Resource manager
studio-cli um group revoke
Revoke a role from a group in tozny.
Synopsis
Revoke a role from a group in tozny.
studio-cli um group revoke [flags]
Options
-h, --help              help for revoke
-n, --name string       Group name
-r, --rolename string   Role name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group search
Search group

<!-- Page 522 -->

Synopsis
Search group.
studio-cli um group search [flags]
Options
-f, --filter string   filter by group name
-h, --help            help for search
-l, --limit float32   The max number of items to return - greater than 0 (default 10)
-o, --order string    Sort order, should be ASC or DESC (default "ASC")
-p, --page float32    Page number  - greater than 0 (default 1)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group update
Update a group.
Synopsis
Update a group.
studio-cli um group update [flags]
Options
-d, --description string   Description for group
-h, --help                 help for update
-n, --new-name string      New name for group
-o, --old-name string      Old name for group

<!-- Page 523 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group user
Group user management commands
Synopsis
Group user management commands.
Options
-h, --help        help for user
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group  - Group management commands
studio-cli um group user assign  - Assign a user to group.
studio-cli um group user list  - Get users from group.
studio-cli um group user revoke  - Revoke a user from group.

<!-- Page 524 -->

studio-cli um group user assign
Assign a user to group.
Synopsis
Assign a user to group.
studio-cli um group user assign [flags]
Options
-g, --group-names strings   Group names
-h, --help                  help for assign
-u, --user-name string      User name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group user  - Group user management commands
studio-cli um group user list
Get users from group.
Synopsis
Get users from group.
studio-cli um group user list [flags]
Options
-h, --help            help for list
-i, --id string       group id
-l, --limit float32   The max number of items to return - greater than 0 (default 10)
-n, --name string     Group name

<!-- Page 525 -->

-o, --order string    Sort order, should be ASC or DESC (default "ASC")
-p, --page float32    Page number  - greater than 0 (default 1)
-s, --sort string     Sort by a table column (id, username, email) (optional) (default
"username")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um group user  - Group user management commands
studio-cli um group user revoke
Revoke a user from group.
Synopsis
Revoke a user from group.
studio-cli um group user revoke [flags]
Options
-g, --group-names strings   Group names
-h, --help                  help for revoke
-u, --username string       User name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 526 -->

## See Also
studio-cli um group user  - Group user management commands
studio-cli um permission
Permission manager
Synopsis
Permission manager
Options
-h, --help        help for permission
-q, --jq string   jq query string override
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um  - User management commands
studio-cli um permission get  - Get User P ermission.
studio-cli um permission get
Get User P ermission.
Synopsis
Get User P ermission.
studio-cli um permission get [flags]

<!-- Page 527 -->

Options
-h, --help             help for get
-i, --user-id string   Wind River User Id, use 'me' for current user. (default "me")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um permission  - Permission manager
studio-cli um role
User management role commands
Synopsis
User management role commands.
Options
-h, --help        help for role
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 528 -->

## See Also
studio-cli um  - User management commands
studio-cli um role attribute  - Display, add or remov e an attribute from a role.
studio-cli um role create  - Create a new role.
studio-cli um role delete  - Delete a role.
studio-cli um role get  - Get role info
studio-cli um role get-me  - Get the roles of the current user.
studio-cli um role list  - Get all roles in the system
studio-cli um role user  - User operations
studio-cli um role attribute
Display, add or remov e an attribute from a role.
Synopsis
Display add or remov e an attribute from a role. If giv en just the role name, all the attributes are
display ed. Using –delete + –key can remov e an attribute. Using –key + –v alue can set or update an
attribute.
This command requires the tozny console admin role.
studio-cli um role attribute [flags]
Options
-d, --delete         Delete key
-h, --help           help for attribute
-k, --key string     Attribute key
-n, --name string    Role name
-v, --value string   Attribute value
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 529 -->

## See Also
studio-cli um role  - User management role commands
studio-cli um role create
Create a new role.
Synopsis
Create a new role.
studio-cli um role create [flags]
Options
-h, --help          help for create
-n, --name string   Name of the role to be created.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um role  - User management role commands
studio-cli um role delete
Delete a role.
Synopsis
Delete a role.
studio-cli um role delete [flags]

<!-- Page 530 -->

Options
-h, --help          help for delete
-i, --id string     Id of the role to be deleted.
-n, --name string   Role name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um role  - User management role commands
studio-cli um role get
Get role info
Synopsis
Gets a speciﬁc role info.
studio-cli um role get [flags]
Options
-h, --help          help for get
-i, --id string     Id of the role to consult.
-n, --name string   Name of the role to consult.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 531 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um role  - User management role commands
studio-cli um role get-me
Get the roles of the current user.
Synopsis
Get the roles of the current user.
studio-cli um role get-me [flags]
Options
-h, --help   help for get-me
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um role  - User management role commands
studio-cli um role list
Get all roles in the system
Synopsis
Get all roles in the system.

<!-- Page 532 -->

studio-cli um role list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um role  - User management role commands
studio-cli um role user
User operations
Synopsis
Perform operations related to users.
Options
-h, --help   help for user
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 533 -->

## See Also
studio-cli um role  - User management role commands
studio-cli um role user list  - Get a list of users that belong to a role.
studio-cli um role user list
Get a list of users that belong to a role.
Synopsis
Search users that belong to a role. Returning a list of users with their data.
studio-cli um role user list [flags]
Options
-h, --help          help for list
-i, --id string     Id of the role to consult.
-n, --name string   Role name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um role user  - User operations
studio-cli um setting
Setting manager
Synopsis
Setting manager

<!-- Page 534 -->

Options
-h, --help   help for setting
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um  - User management commands
studio-cli um setting get  - Get current user settings.
studio-cli um setting list  - Get all system settings
studio-cli um setting update  - Update current user settings.
studio-cli um setting get
Get current user settings.
Synopsis
Get current user settings.
studio-cli um setting get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 535 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um setting  - Setting manager
studio-cli um setting list
Get all system settings
Synopsis
Get all system settings
studio-cli um setting list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um setting  - Setting manager
studio-cli um setting update
Update current user settings.
Synopsis
Update current user settings.
studio-cli um setting update [flags]

<!-- Page 536 -->

Options
-h, --help                     help for update
-s, --setting stringToString   Setting to update. For example: --setting
SettingId1=SettingValueId1 --setting SettingId2=SettingValueId2 (default [])
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um setting  - Setting manager
studio-cli um user
Tozny user commands
Synopsis
Tozny user commands.
Options
-h, --help        help for user
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 537 -->

## See Also
studio-cli um  - User management commands
studio-cli um user audit  - Create an audit entry for user actions.
studio-cli um user create  - Create a new user
studio-cli um user delete  - Delete an user.
studio-cli um user disable  - Set user enabled to false
studio-cli um user enable  - Set user enabled to true
studio-cli um user get  - Get user info with details.
studio-cli um user get-groups  - Get groups from user.
studio-cli um user get-resource  - Get the list of resources a user has access.
studio-cli um user group  - User group commands
studio-cli um user list  - Get a list of users in the system.
studio-cli um user proﬁle  - User personal proﬁle manager
studio-cli um user read  - Read user data.
studio-cli um user reset-password  - Request a password reset for a speciﬁc user
studio-cli um user role  - User role commands
studio-cli um user search  - Search user by ﬁltering and paging.
studio-cli um user audit
Create an audit entry for user actions.
Synopsis
Create and audit on the DB for the action of a certain user.
studio-cli um user audit [flags]
Options
-d, --description string   description
-h, --help                 help for audit
-i, --id string            user id
-n, --name string          user name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 538 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user create
Create a new user
Synopsis
Create a new user.
studio-cli um user create [flags]
Examples
rbac user create --name "test_name" --email "test_name@company.com"  --first-name
test_firstName --last-name test_lastName --last-failed-login "2025-09-12T12:46:17.169Z"  --
last-login "2025-09-13T12:46:17.169Z" --permissions "{"key": "permissionKeyInfo1","name":
"name1","permissionId": "permissionId1"}" --permissions "{"key":
"permissionKeyInfo2","name": "name2","permissionId": "permissionId2"}"
Options
--department string                 department
-e, --email string                      email
-f, --first-name string                 firstName
-h, --help                              help for create
-i, --id string                         user id
--is-bot                            isBot
--last-failed-login string          lastFailedLogin
--last-login string                 last-login
-l, --last-name string                  lastName
--license-action-date string        licenseActionDate
--license-id string                 licenseId
--license-last-assign-date string   licenseLastAssignDate
--location string                   location
--mobile-phone-number string        mobilePhoneNumber
-n, --name string                       user name
--nickname string                   nickname
--nolicense                         Do not assign a WR Studio at the time of user
creation

<!-- Page 539 -->

-p, --password string                   password
--permissions stringArray           Array of Permissions, --permissions "{"key":
"key","name": "projectName","permissionId": "permissionId"}"
--phone-number string               phoneNumber
--position string                   position
--tenant-id string                  tenantId
--tozny-id string                   toznyId
-t, --type string                       user type
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user delete
Delete an user.
Synopsis
Delete an user.
studio-cli um user delete [flags]
Options
-h, --help          help for delete
-n, --name string   name of the user to delete.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 540 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user disable
Set user enabled to false
Synopsis
“Set user enabled to false, a disabled user cannot login
studio-cli um user disable [flags]
Options
-h, --help          help for disable
-u, --user string   User name or ID
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user enable
Set user enabled to true

<!-- Page 541 -->

Synopsis
“Set user enabled to true, a disabled user cannot login
studio-cli um user enable [flags]
Options
-h, --help          help for enable
-u, --user string   User name or ID
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user get
Get user info with details.
Synopsis
Get user info with details (roles, settings, perms, etc).
studio-cli um user get [flags]
Options
-h, --help          help for get
-i, --id string     Id of the user to consult
-n, --name string   Name of the user to consult
-p, --with-pic      Boolean flag in case user picture is needed

<!-- Page 542 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user get-groups
Get groups from user.
Synopsis
Get groups from user.
studio-cli um user get-groups [flags]
Options
-h, --help          help for get-groups
-n, --name string   user name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands

<!-- Page 543 -->

studio-cli um user get-resource
Get the list of resources a user has access.
Synopsis
Get the list of resources a user has access.
studio-cli um user get-resource [flags]
Options
-h, --help          help for get-resource
-i, --id string     user name or id
-w, --wrrn string   resource wrrn
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user group
User group commands
Synopsis
User group commands.
Options
-h, --help   help for group

<!-- Page 544 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user group assign  - Assign a user to group.
studio-cli um user group revoke  - Revoke a user from group.
studio-cli um user group assign
Assign a user to group.
Synopsis
Assign a user to group.
studio-cli um user group assign [flags]
Options
-g, --group-names strings   Group names
-h, --help                  help for assign
-u, --username string       User name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 545 -->

## See Also
studio-cli um user group  - User group commands
studio-cli um user group revoke
Revoke a user from group.
Synopsis
Revoke a user from group.
studio-cli um user group revoke [flags]
Options
-g, --group-names strings   Group names
-h, --help                  help for revoke
-u, --username string       User name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user group  - User group commands
studio-cli um user list
Get a list of users in the system.
Synopsis
Get a list of users by the tenant id of the current user. Also v eriﬁes that the user is an admin.
studio-cli um user list [flags]

<!-- Page 546 -->

Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user profile
User personal proﬁle manager
Synopsis
User personal proﬁle manager
Options
-h, --help        help for profile
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 547 -->

## See Also
studio-cli um user  - Tozny user commands
studio-cli um user proﬁle get  - Get current user proﬁle.
studio-cli um user proﬁle picture  - User proﬁle picture manager
studio-cli um user proﬁle update  - Update current user proﬁle.
studio-cli um user profile get
Get current user proﬁle.
Synopsis
Get the proﬁle of the current user.
studio-cli um user profile get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user proﬁle  - User personal proﬁle manager
studio-cli um user profile picture
User proﬁle picture manager
Synopsis
User proﬁle picture manager

<!-- Page 548 -->

Options
-h, --help        help for picture
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user proﬁle  - User personal proﬁle manager
studio-cli um user proﬁle picture get  - Get user proﬁle picture.
studio-cli um user proﬁle picture update  - Update user proﬁle picture.
studio-cli um user profile picture get
Get user proﬁle picture.
Synopsis
Get proﬁle picture from user.
studio-cli um user profile picture get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 549 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user proﬁle picture  - User proﬁle picture manager
studio-cli um user profile picture update
Update user proﬁle picture.
Synopsis
Set a new proﬁle picture for a user.
studio-cli um user profile picture update [flags]
Options
-h, --help             help for update
-p, --picture string   picture
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user proﬁle picture  - User proﬁle picture manager
studio-cli um user profile update
Update current user proﬁle.

<!-- Page 550 -->

Synopsis
Update the proﬁle of the current user. Information needed to update user. - image should be a v alid
base64 string - email & username are not updatable ﬁelds.
studio-cli um user profile update [flags]
Options
--department string   department
-e, --email string        email is not updatable field.
-f, --first-name string   first name
-h, --help                help for update
-l, --last-name string    last name
--location string     location
-m, --mobile string       mobile phone number
-n, --name string         name is not updatable field.
--nickname string     nickname
-p, --phone string        phone number
--position string     position
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user proﬁle  - User personal proﬁle manager
studio-cli um user read
Read user data.
Synopsis
Read user data.
studio-cli um user read [flags]

<!-- Page 551 -->

Options
-h, --help          help for read
-i, --id string     id of the user to consult.
-n, --name string   name of the user to consult.
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user reset-password
Request a password reset for a speciﬁc user
Synopsis
Request a password reset URL for another end user other than yourself or directly set a password for
an end user.
The –password or –ask option can be used to force set a password which is at least 10 characters long.
studio-cli um user reset-password [flags]
Options
-a, --ask               Use a password prompt to enter the password
-h, --help              help for reset-password
-p, --password string   Force set new password instead of generating a password link
-u, --username string   User name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 552 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user role
User role commands
Synopsis
User role commands.
Options
-h, --help   help for role
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user  - Tozny user commands
studio-cli um user role assign  - Add an av ailable role to a username
studio-cli um user role revoke  - Remov e an assigned role from a username
studio-cli um user role assign
Add an av ailable role to a username

<!-- Page 553 -->

Synopsis
Add an av ailable role to a username.
This command currently requires a role that allows access to the tozny admin dashboard.
studio-cli um user role assign [flags]
Options
-h, --help              help for assign
-r, --role strings      Role name
-u, --username string   User name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user role  - User role commands
studio-cli um user role revoke
Remov e an assigned role from a username
Synopsis
Remov e an assigned role from a username.
This command currently requires a role that allows access to the tozny admin dashboard.
studio-cli um user role revoke [flags]
Options
-h, --help              help for revoke
-r, --role strings      Role name
-u, --username string   User name

<!-- Page 554 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli um user role  - User role commands
studio-cli um user search
Search user by ﬁltering and paging.
Synopsis
Search user with ﬁltering for name, last name and username & pagination.
studio-cli um user search [flags]
Options
-f, --filter string   The filter option should be by first or last name or username or
email
-h, --help            help for search
-l, --limit float32   The max number of items to return - (0 for all) (default 10)
-o, --order string    Sort order, should be ASC or DESC (default "ASC")
-p, --page float32    The page number - greater than 0 (default 1)
-s, --sort string     Sort by a table column (username, firstName, lastName, email)
(default "username")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 555 -->

## See Also
studio-cli um user  - Tozny user commands
studio-cli vlab
Physical/V irtual target control commands
Synopsis
Physical/V irtual target control commands.
Options
-h, --help   help for vlab
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli v lab physical  - Manage physical targets.
studio-cli v lab property  - Manage v lab properties.
studio-cli v lab reserv ation  - Manage reserv ation.
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab physical
Manage physical targets.
Synopsis
Manage physical targets.

<!-- Page 556 -->

Options
-h, --help   help for physical
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab - Physical/V irtual target control commands
studio-cli v lab physical create  - Create a physical target deﬁnition using a json ﬁle (requires admin).
studio-cli v lab physical delete  - Delete a physical target deﬁnition by id (requires admin)
studio-cli v lab physical info  - Show all details of a particular target
studio-cli v lab physical pow eroﬀ  - Set pow er oﬀ physical target
studio-cli v lab physical pow eron  - Set pow er on physical target
studio-cli v lab physical reboot  - Reboot physical target by id (coming soon)
studio-cli v lab physical reserv e - Create Physical target reserv ation now or in the future.
studio-cli v lab physical search  - Search for physical targets
studio-cli v lab physical slc  - Manage SLC (Studio Lab Connect) targets.
studio-cli v lab physical status-change  - Enable or Disable physical targets
studio-cli v lab physical unreserv e - Unreserv e Physical target immediately.
studio-cli vlab physical create
Create a physical target deﬁnition using a json ﬁle (requires admin).
Synopsis
Create a physical target definition using a json file (requires
admin). Note: Below examples contain only required fields.

<!-- Page 557 -->

Empty , null or false values indicate that those fields are optional
but they must be part of the Json structure.
Json file Example for Physical Lab T arget:
{ “target”: { “name”: “i-am-a-physical-lab-target”, “barcode”:
“00000001”, “labId”: “8fcff1eb-8706-4cf0-bb80-ac52c0ce6059”,
“cpuConfigId”: “727c160e-57a8-4fda-9447-cf7990561fba”,
“architectureId”: “9d7e3f7c-fdae-4eb5-930d-00274f0606cc”,
“externalId”: null, “manufacturer”: “”, “model”: “”, “rack”: “”,
“bootDevice”: “”, “slot”: “”, “memorySize”: null, “flashSize”:
null, “bootRomV ersion”: “”, “bootBuild”: “”, “bios”: “”,
“firmware”: “”, “endianness”: “”, “comment”: “”,
“bootServerId”: null, “bspId”: null, “connectionT ypeId”: null,
“gatewayId”: null }, “terminalServerHasPort”: [ {
“terminalServerId”: “2c86302b-1bf3-4f74-a76b-f86601c2756c”,
“port”: 22, “isDefault”: true, “baudRate”: null, “portNote”: null }
], “targetHasInterface”: [ { “interfaceId”: “305bf164-a1bc-485a-
bd99-296b51085ce4”, “ipv4Address”: “192.168.100.2”,
“subnetMask”: “0.0.0.0”, “ipv6Address”: null, “ipv6Alias”: null,
“gateway”: null, “macAddress”: null, “speed”: null,
“isBootable”: false, “isConnected”: false, “sshEnable”: false,
“sshUsername”: null, “sshPort”: null, “webEnable”: false,
“webUrl”: null, “ipmiEnable”: false, “ipmiUsername”: null,
“ipmiPassword”: null, “ipmiPort”: null, “isDefault”: true } ],
“kvmHasPort”: null, “pduHasPort”: [], “targetHasExtraInfo”: [] }
Json file Example for Studio Lab Connect T arget:
{ “target”: { “name”: “i-am-a-slc-target”, “barcode”: “00000002”, “labId”: “8fcﬀ1eb-8706-4cf0-bb80-
ac52c0ce6059”, “cpuConﬁgId”: “727c160e-57a8-4fda-9447-cf7990561fba”, “architectureId”: “9d7e3f7c-
fdae-4eb5-930d-00274f0606cc”, “connectionTypeId”: “be43cbc9-381a-4214-9ed3-1c02a74bba75”,
“gatew ayId”: “83c0c36f-7e2b-4557-9b16-979716dfb640”, “externalId”: null, “manufacturer”: “”,
“model”: “”, “rack”: “”, “bootDevice”: “”, “slot”: “”, “memorySize”: null, “ﬂashSize”: null,
“bootRomV ersion”: “”, “bootBuild”: “”, “bios”: “”, “ﬁrmw are”: “”, “endianness”: “”, “comment”: “”,
“bootServ erId”: null, “bspId”: null }, “labConnectConﬁg”: { “labConnectData”: {
“connectionTypeName”: “ssh”, “targetHost”: “192.168.100.5”, “targetUser”: “admin”, “port”: null } },
“kvmHasPort”: null, “pduHasPort”: [], “targetHasInterface”: [], “targetHasExtraInfo”: [] }
studio-cli vlab physical create [flags]
Options
-b, --barcode string   the barcode to give the new target
-f, --file string      config file for target definition [json]

<!-- Page 558 -->

-h, --help             help for create
-q, --jq string        jq query string
-n, --name string      the name to give the new target
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical  - Manage physical targets.
studio-cli vlab physical delete
Delete a physical target deﬁnition by id (requires admin)
Synopsis
Delete a physical target deﬁnition by id (requires admin)
studio-cli vlab physical delete [flags]
Options
-h, --help        help for delete
-i, --id string   target id to delete
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical  - Manage physical targets.

<!-- Page 559 -->

studio-cli vlab physical info
Show all details of a particular target
Synopsis
Show all details of a particular target.
studio-cli vlab physical info [flags]
Options
-h, --help        help for info
-i, --id string   target  id
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical  - Manage physical targets.
studio-cli vlab physical poweroff
Set pow er oﬀ physical target
Synopsis
Set pow er oﬀ physical target
studio-cli vlab physical poweroff [flags]

<!-- Page 560 -->

Options
-h, --help        help for poweroff
-i, --id string   target  id
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical  - Manage physical targets.
studio-cli vlab physical poweron
Set pow er on physical target
Synopsis
Set pow er on physical target
studio-cli vlab physical poweron [flags]
Options
-h, --help        help for poweron
-i, --id string   target  id
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 561 -->

## See Also
studio-cli v lab physical  - Manage physical targets.
studio-cli vlab physical reboot
Reboot physical target by id (coming soon)
Synopsis
Reboot physical target by id (coming soon)
studio-cli vlab physical reboot [flags]
Options
-h, --help        help for reboot
-i, --id string   target id to reboot
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical  - Manage physical targets.
studio-cli vlab physical reserve
Create Physical target reserv ation now or in the future.
Synopsis
Create Physical target reserv ation now or in the future.
studio-cli vlab physical reserve [flags]

<!-- Page 562 -->

Options
-e, --end-datetime string     End datetime in 'YYYY-MM-DD HH:MM' format
-h, --help                    help for reserve
-H, --hours string            Number of hours for reservation (default "24")
-i, --id string               target  id
-q, --jq string               jq query string
-s, --start-datetime string   Start datetime in 'YYYY-MM-DD HH:MM' format
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical  - Manage physical targets.
studio-cli vlab physical search
Search for physical targets
Synopsis
Search for physical targets which can be ﬁltered by id, name, barcode, architecture or reserv ation.
studio-cli vlab physical search [flags]
Options
-a, --all               Return all available results
-b, --barcode string    filter by barcode
-h, --help              help for search
-i, --id string         filter by target id
-q, --jq string         jq query string
-l, --limit int         Entries per page, 0 for all entries (default 10)
-n, --name string       filter by name
-o, --offset int        Offset into search list (default 1)
-p, --platform string   [Physical|Lab-Connect]

<!-- Page 563 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical  - Manage physical targets.
studio-cli vlab physical slc
Manage SLC (Studio Lab Connect) targets.
Synopsis
Manage SLC (Studio Lab Connect) targets.
Options
-h, --help   help for slc
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 564 -->

## See Also
studio-cli v lab physical  - Manage physical targets.
studio-cli v lab physical slc connect  - SSH connection to the SLC target
studio-cli v lab physical slc ﬁle  - Manage gatew ay ﬁle transfers
studio-cli v lab physical slc gatew ay - Manage gatew ay administration
studio-cli v lab physical slc pow er-oﬀ  - Pow er oﬀ SLC target
studio-cli v lab physical slc pow er-on  - Pow er on SLC target
studio-cli v lab physical slc reboot  - Reboot SLC target
studio-cli v lab physical slc target  - Manage SLC target actions
studio-cli vlab physical slc connect
SSH connection to the SLC target
Synopsis
SSH connection to the SLC target
studio-cli vlab physical slc connect [flags]
Options
-j, --config-file string            The path of the json file that contains the DIS
certificates
-c, --connection-type string        Type of the connection to be passed
[serial|ssh|telnet|ssh-gateway]
-x, --execute-command string        Additional commands for the ssh connection
-g, --gwid string                   The name of the gateway to establish SSH connection
-h, --help                          help for connect
-r, --port string                   The port to be used for the SSH connection
-n, --remain-connected              Remain connection alive after executing a command
(only for ssh connection)
-e, --target-baud-rate string       The baud rate value for the serial connection
-b, --target-data-bits string       The data bits value for the serial connection (only
supported by putty client)
-f, --target-flow-control string    The flow control value for the serial connection
(only supported by putty client)
-o, --target-host string            IP of the host or serial port device name to
establish the connection
-p, --target-name string            The name of the SLC target
-z, --target-os string              The operating system of the SLC target
-a, --target-parity string          The parity value for the serial connection (only
supported by putty client)
-k, --target-port string            Optional port for the ssh/telnet connection
-s, --target-serial-client string   The serial client for the serial connection
[minicom|putty]

<!-- Page 565 -->

-i, --target-stop-bits string       The stop bits value for the serial connection (only
supported by putty client)
-u, --target-user string            Target user of the host to establish an SSH
connection
-t, --token string                  Token to pass to authorize request from studio-cli
-d, --url string                    The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc  - Manage SLC (Studio Lab Connect) targets.
studio-cli vlab physical slc file
Manage gatew ay ﬁle transfers
Synopsis
Manage gatew ay ﬁle transfers.
Options
-h, --help   help for file
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 566 -->

## See Also
studio-cli v lab physical slc  - Manage SLC (Studio Lab Connect) targets.
studio-cli v lab physical slc ﬁle cancel  - Cancel ﬁle transfer
studio-cli v lab physical slc ﬁle conﬁguration  - Set the conﬁgurations for the ﬁle handling
studio-cli v lab physical slc ﬁle download  - Download ﬁles to the gatew ay
studio-cli v lab physical slc ﬁle get  - Get ﬁles from the gatew ay
studio-cli v lab physical slc ﬁle list  - List ﬁles on the gatew ay
studio-cli v lab physical slc ﬁle target-script-download  - File transfer directly to the target
studio-cli v lab physical slc ﬁle target-script-get  - File transfer directly from the target
studio-cli v lab physical slc ﬁle upload  - Upload ﬁles from the gatew ay to Studio Artifacts
studio-cli vlab physical slc file cancel
Cancel ﬁle transfer
Synopsis
Cancel ﬁle transfer from/to DIS/Gatew ay
studio-cli vlab physical slc file cancel [flags]
Options
-f, --config-file string   The json file that contains the DIS certificates
--files json           Array of strings with list of files to cancel transfer
(default null)
-n, --gwid string          The name of the gateway target
-h, --help                 help for cancel
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 567 -->

## See Also
studio-cli v lab physical slc ﬁle  - Manage gatew ay ﬁle transfers
studio-cli vlab physical slc file configuration
Set the conﬁgurations for the ﬁle handling
Synopsis
Set the conﬁgurations for the ﬁle handling
studio-cli vlab physical slc file configuration [flags]
Options
-c, --config-file string   The json file that contains the DIS certificates
-g, --gwid string          The name of the Gateway
-h, --help                 help for configuration
-s, --size-limit string    The size limit for the files sent to the Gateway
-t, --token string         The token to authorize the request from studio cli
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc ﬁle  - Manage gatew ay ﬁle transfers
studio-cli vlab physical slc file download
Download ﬁles to the gatew ay
Synopsis
Download ﬁles to the gatew ay

<!-- Page 568 -->

studio-cli vlab physical slc file download [flags]
Options
-f, --config-file string         The json file that contains the DIS certificates
-p, --download-path string       The path to be used to store the files on the Gateway.
If DownloadPath is not specified the default path will be /tftp
--files json                 Array of strings with path/url of files to download
(default null)
-n, --gwid string                The name of the gateway target
-h, --help                       help for download
-i, --hide-progress-bar          Hide progress bar during file transfers
-l, --location string            Location of files: artifacts | local
-r, --ram-resource-name string   The name of the RAM resource for the user secret access
management (group access)
-s, --secret-path string         The path of the user secret in vault (user access)
-u, --url string                 The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc ﬁle  - Manage gatew ay ﬁle transfers
studio-cli vlab physical slc file get
Get ﬁles from the gatew ay
Synopsis
Get ﬁles from the gatew ay
studio-cli vlab physical slc file get [flags]
Options
-f, --config-file string   The json file that contains the DIS certificates
--files json           Array of strings with list of files to download from the
Gateway (default null)

<!-- Page 569 -->

-n, --gwid string          The name of the gateway target
-h, --help                 help for get
-i, --hide-progress-bar    Hide progress bar during file transfers
-p, --source-path string   The path to be used to get the files on the Gateway. If
source_path is not specified the default path will be /tftp
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc ﬁle  - Manage gatew ay ﬁle transfers
studio-cli vlab physical slc file list
List ﬁles on the gatew ay
Synopsis
List ﬁles on the gatew ay
studio-cli vlab physical slc file list [flags]
Options
-c, --config-file string   The json file that contains the DIS certificates
-g, --gwid string          The name of the gateway target
-h, --help                 help for list
-p, --list-path string     The path to be used to get the file list on the Gateway. If
list-path is not specified the default path will be /tftp
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode

<!-- Page 570 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc ﬁle  - Manage gatew ay ﬁle transfers
studio-cli vlab physical slc file target-script-download
File transfer directly to the target
Synopsis
File transfer directly to the target
studio-cli vlab physical slc file target-script-download [flags]
Options
-p, --action-script-path string   The path to be action script that will be used to
transfer files to the target
--arguments json              Array of strings with the arguments for the script
execution. (The Target User is required as first argument, the IP Address is required as
second argument, and the Intended Directory Destination is required as third argument)
(default null)
-f, --config-file string          The json file that contains the DIS certificates
-m, --download-path string        The path to be used to get the file on the Gateway. If
download-path is not specified the default path will be /tftp/
--files json                  Array of strings with path/url of files to send to the
target (default null)
-g, --gwid string                 The name of the gateway
-h, --help                        help for target-script-download
-i, --hide-script-output          Hide the output of the action script
-l, --location string             Location of files: artifacts | local
-r, --ram-resource-name string    The name of the RAM resource for the user secret access
management (group access)
-s, --secret-path string          The path of the user secret in vault (user access)
-t, --target-name string          The json file that contains the DIS certificates
-u, --url string                  The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 571 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc ﬁle  - Manage gatew ay ﬁle transfers
studio-cli vlab physical slc file target-script-get
File transfer directly from the target
Synopsis
File transfer directly from the target
studio-cli vlab physical slc file target-script-get [flags]
Options
-p, --action-script-path string   The path to be action script that will be used to get
files from the target
--arguments json              Array of strings with the arguments for the script
execution. (The Target User is required as first argument, the IP Address is required as
second argument, and the Intended Directory Destination is required as third argument)
(default null)
-K, --artifact-key string         The access key of the user in Artifacts
-P, --artifact-path string        The path in Artifacts where the files will be uploaded
-U, --artifact-user string        The user in Artifacts
-f, --config-file string          The json file that contains the DIS certificates
--files json                  Array of strings with path/url of files to get from the
target (default null)
-g, --gwid string                 The name of the gateway
-h, --help                        help for target-script-get
-i, --hide-script-output          Hide the output of the action script
-l, --location string             Location of files: artifacts | local
-r, --ram-resource-name string    The name of the RAM resource for the user secret access
management (group access)
-s, --secret-path string          The path of the user secret in vault (user access)
-t, --target-name string          The target name
-u, --url string                  The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 572 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc ﬁle  - Manage gatew ay ﬁle transfers
studio-cli vlab physical slc file upload
Upload ﬁles from the gatew ay to Studio Artifacts
Synopsis
Upload ﬁles from the gatew ay to Studio Artifacts
studio-cli vlab physical slc file upload [flags]
Options
-m, --artifacts-path string      The path in Artifacts where the files will be uploaded
-c, --config-file string         The json file that contains the DIS certificates
--files json                 Array of strings with path/url of files to download
(default null)
-g, --gwid string                The name of the gateway target
-h, --help                       help for upload
-i, --hide-progress-bar          Hide progress bar during file transfers
-r, --ram-resource-name string   The name of the RAM resource for the user secret access
management (group access)
-s, --secret-path string         The path of the user secret in vault (user access)
-p, --source-path string         The path to be used to get the files on the Gateway. If
source_path is not specified the default path will be /tftp
-t, --token string               token to authorize the request from studio cli
-u, --url string                 The url of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc ﬁle  - Manage gatew ay ﬁle transfers

<!-- Page 573 -->

studio-cli vlab physical slc gateway
Manage gatew ay administration
Synopsis
Manage gatew ay administration.
Options
-h, --help   help for gateway
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc  - Manage SLC (Studio Lab Connect) targets.
studio-cli v lab physical slc gatew ay associate  - Associate a SLC target with a gatew ay
studio-cli v lab physical slc gatew ay disconnect  - Disconnect a gatew ay from the DIS
studio-cli v lab physical slc gatew ay extend-time  - Extend av ailable connection time of a gatew ay
studio-cli v lab physical slc gatew ay list  - List the registered gatew ays
studio-cli v lab physical slc gatew ay provision  - Provision the gatew ay conﬁg ﬁle
studio-cli v lab physical slc gatew ay register  - Register a gatew ay
studio-cli v lab physical slc gatew ay unregister  - Unregister a gatew ay
studio-cli vlab physical slc gateway associate
Associate a SLC target with a gatew ay
Synopsis
Associate a SLC target with a gatew ay
studio-cli vlab physical slc gateway associate [flags]

<!-- Page 574 -->

Options
-f, --config-file string   The path of the json file that contains the DIS certificates
-g, --gwid string          The name of the gateway that will be associated with the
target
-h, --help                 help for associate
-n, --target-name string   The name of the SLC target
-t, --token string         Token to authorize the request from studio cli
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc gatew ay - Manage gatew ay administration
studio-cli vlab physical slc gateway disconnect
Disconnect a gatew ay from the DIS
Synopsis
Disconnect a gatew ay from the DIS
studio-cli vlab physical slc gateway disconnect [flags]
Options
-c, --config-file string   The json file that contains the DIS certificates
-g, --gwid string          The name of the gateway to disconnect
-h, --help                 help for disconnect
-t, --token string         The token to authorize the request from studio cli
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses

<!-- Page 575 -->

--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc gatew ay - Manage gatew ay administration
studio-cli vlab physical slc gateway extend-time
Extend av ailable connection time of a gatew ay
Synopsis
Extend av ailable connection time of a gatew ay
studio-cli vlab physical slc gateway extend-time [flags]
Options
-c, --config-file string               The json file that contains the DIS certificates
-m, --connection-time string           Extend the gateway connection time to the DIS in
minutes
-d, --connection-time-days string      Extend the gateway connection time to the DIS in
days
-p, --connection-time-hours string     Extend the gateway connection time to the DIS in
hours
-n, --connection-time-minutes string   Extend the gateway connection time to the DIS in
minutes
-i, --connection-time-unlimited        Extend the gateway connection time to the DIS to
be indefinite. By default the value is false
-g, --gwid string                      The name of the gateway to register
-h, --help                             help for extend-time
-t, --token string                     Token to authorize the request from studio cli
-u, --url string                       The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 576 -->

## See Also
studio-cli v lab physical slc gatew ay - Manage gatew ay administration
studio-cli vlab physical slc gateway list
List the registered gatew ays
Synopsis
List the registered gatew ays
studio-cli vlab physical slc gateway list [flags]
Options
-g, --gwid string   The name of the gateway
-h, --help          help for list
--plain-text    Change the output format to plain text, JSON otherwise
-u, --url string    The URL of the DIS server
-s, --user string   The user (that registered a gateway) to filter the list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc gatew ay - Manage gatew ay administration
studio-cli vlab physical slc gateway provision
Provision the gatew ay conﬁg ﬁle
Synopsis
Provision the gatew ay conﬁg ﬁle

<!-- Page 577 -->

studio-cli vlab physical slc gateway provision [flags]
Options
-f, --config-file string   The json file path that contains the DMS certificates
-n, --gwid string          The name of the gateway
-h, --help                 help for provision
-u, --url string           The URL of the DMS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc gatew ay - Manage gatew ay administration
studio-cli vlab physical slc gateway register
Register a gatew ay
Synopsis
Register a gatew ay
studio-cli vlab physical slc gateway register [flags]
Options
-c, --config-file string               The json file that contains the DIS certificates
-t, --connection-time string           Set the connection time of the gateway to the DIS
in minutes. By default the connection time is 30 minutes
-d, --connection-time-days string      Set the connection time of the gateway to the DIS
in days
-p, --connection-time-hours string     Set the connection time of the gateway to the DIS
in hours
-m, --connection-time-minutes string   Set the connection time of the gateway to the DIS
in minutes. By default the connection time is 30 minutes
-i, --connection-time-unlimited        Set the connection time of the gateway to the DIS
to be indefinite. By default the value is false
-g, --gwid string                      The name of the gateway to register

<!-- Page 578 -->

-h, --help                             help for register
-u, --url string                       The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc gatew ay - Manage gatew ay administration
studio-cli vlab physical slc gateway unregister
Unregister a gatew ay
Synopsis
Unregister a gatew ay
studio-cli vlab physical slc gateway unregister [flags]
Options
-c, --config-file string   The json file that contains the DIS certificates
-g, --gwid string          The name of the gateway to unregister
-h, --help                 help for unregister
-t, --token string         The token to authorize the request from studio cli
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 579 -->

## See Also
studio-cli v lab physical slc gatew ay - Manage gatew ay administration
studio-cli vlab physical slc power-off
Pow er oﬀ SLC target
Synopsis
Pow er oﬀ SLC target
studio-cli vlab physical slc power-off [flags]
Options
-f, --config-file string       The path of the json file that contains the DIS
certificates
-c, --connection-type string   Pass the connection type(gpio/snmp) needed for Studio
connect device
-p, --gpio-pin string          Pass the GPIO pin that needs to be controlled in the
Studio connect device
-g, --gwid string              The name of the gateway to establish connection
-h, --help                     help for power-off
-i, --pdu-ip string            Pass the PDU ip to be controlled in the Studio connect
device
-o, --pdu-port string          Pass the PDU port that needs to be controlled in the
Studio connect device
-s, --target-name string       The name of the SLC target
-t, --token string             Token to authorize the request from studio cli
-u, --url string               The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc  - Manage SLC (Studio Lab Connect) targets.

<!-- Page 580 -->

studio-cli vlab physical slc power-on
Pow er on SLC target
Synopsis
Pow er on SLC target
studio-cli vlab physical slc power-on [flags]
Options
-f, --config-file string       The path of the json file that contains the DIS
certificates
-c, --connection-type string   Pass the connection type(gpio/snmp) needed for Studio
connect device
-p, --gpio-pin string          Pass the gpio pin that needs to be controlled in the
Studio connect device
-g, --gwid string              The name of the gateway to establish connection
-h, --help                     help for power-on
-i, --pdu-ip string            Pass the PDU ip to be controlled in the Studio connect
device
-o, --pdu-port string          Pass the PDU port that needs to be controlled in the
Studio connect device
-s, --target-name string       The name of the SLC target
-t, --token string             Token to authorize the request from studio cli
-u, --url string               The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc  - Manage SLC (Studio Lab Connect) targets.
studio-cli vlab physical slc reboot
Reboot SLC target

<!-- Page 581 -->

Synopsis
Reboot SLC target
studio-cli vlab physical slc reboot [flags]
Options
-f, --config-file string       The path of the json file that contains the DIS
certificates
-c, --connection-type string   Pass the connection type(gpio/snmp) needed for Studio
connect device
-p, --gpio-pin string          Pass the gpio pin that needs to be controlled in the
Studio connect device
-g, --gwid string              The name of the gateway to establish connection
-h, --help                     help for reboot
-i, --pdu-ip string            Pass the PDU ip to be controlled in the Studio connect
device
-o, --pdu-port string          Pass the PDU port that needs to be controlled in the
Studio connect device
-s, --target-name string       The name of the SLC target
-t, --token string             Token to authorize the request from studio cli
-u, --url string               The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc  - Manage SLC (Studio Lab Connect) targets.
studio-cli vlab physical slc target
Manage SLC target actions
Synopsis
Manage SLC target actions.

<!-- Page 582 -->

Options
-h, --help   help for target
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc  - Manage SLC (Studio Lab Connect) targets.
studio-cli v lab physical slc target act  - Specify a user-deﬁned script on the gatew ay to perform an
action on the target
studio-cli v lab physical slc target get-v ersion  - Get the target system v ersion from a user-deﬁned
script and update the physical target information
studio-cli v lab physical slc target health-check  - Specify a user-deﬁned script on the gatew ay to
perform a health check connection to the target
studio-cli v lab physical slc target kill  - Allows target action script execution in progress to be killed
studio-cli v lab physical slc target list  - List action scripts and their PIDs that are currently executing
on the target
studio-cli v lab physical slc target log  - Specify an executed script PID on the gatew ay to get the
output log
studio-cli vlab physical slc target act
Specify a user-deﬁned script on the gatew ay to perform an action on the target
Synopsis
Specify a user-deﬁned script on the gatew ay to perform an action on the target
studio-cli vlab physical slc target act [flags]
Options
-a, --action-path string         Pass the path of the script that needs to be executed at
the gateway

<!-- Page 583 -->

-b, --back-exec string           Optional: Execute script on background true|false
-o, --back-exec-timeout string   Optional: Stop background execution after timeout
specified in minutes
-f, --config-file string         The path of the json file that contains the DIS
certificates
-e, --exec-command string        Optional: Specify command execution for the action
script (<exec-command> <action-script>) (eg: 'bash test.sh', 'python test.py'). If not
specified, default value is 'bash'.
-p, --gpio-pin string            Pass the gpio pin that needs to be controlled in the
Studio connect device
-l, --gpio-toggle string         Pass the value as on or off for the gpio pin to be
turned on or off accordingly
-g, --gwid string                The name of the gateway to establish connection
-h, --help                       help for act
--parameters json            Array of strings with the parameters for the script
execution (default null)
-s, --show-progress string       Optional: Show progress during script execution
true|false
-r, --target-name string         The name of the SLC target
-t, --token string               Token to authorize the request from studio cli
-u, --url string                 The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc target  - Manage SLC target actions
studio-cli vlab physical slc target get-version
Get the target system v ersion from a user-deﬁned script and update the physical target information
Synopsis
Get the target system v ersion from a user-deﬁned script and update the physical target information
studio-cli vlab physical slc target get-version [flags]

<!-- Page 584 -->

Options
-f, --config-file string   The path of the json file that contains the DIS certificates
-g, --gwid string          The name of the gateway that will be associated with the
target
-h, --help                 help for get-version
-s, --script-path string   The path of the script that will retrieve the system version
of the target
-n, --target-name string   The name of the SLC target
-t, --token string         Token to authorize the request from studio cli
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc target  - Manage SLC target actions
studio-cli vlab physical slc target health-check
Specify a user-deﬁned script on the gatew ay to perform a health check connection to the target
Synopsis
Specify a user-deﬁned script on the gatew ay to perform a health check connection to the target
studio-cli vlab physical slc target health-check [flags]
Options
-a, --action-path string   Pass the path of the health check script that needs to be
executed on the gateway
-f, --config-file string   The path of the json file that contains the DIS certificates
-g, --gwid string          The name of the gateway to establish connection
-h, --help                 help for health-check
--parameters json      Array of strings with the arguments for the health check
script execution (For HIL health check IP Address as first argument is required, SSH Port
as second argument and Timeout in seconds as third argument are optional) (default null)
-r, --target-name string   The name of the SLC target

<!-- Page 585 -->

-t, --token string         Token to authorize the request from studio cli
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc target  - Manage SLC target actions
studio-cli vlab physical slc target kill
Allows target action script execution in progress to be killed
Synopsis
Allows target action script execution in progress to be killed
studio-cli vlab physical slc target kill [flags]
Options
-a, --action-path string   The path of the target action script that is currently running
-f, --config-file string   The json file that contains the DIS certificates
-g, --gwid string          The name of the Gateway
-h, --help                 help for kill
-p, --pid string           The pid (process ID) of the target action script that is
currently running
-n, --target-name string   The name of the SLC target
-t, --token string         The token to authorize the request from studio cli
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 586 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc target  - Manage SLC target actions
studio-cli vlab physical slc target list
List action scripts and their PIDs that are currently executing on the target
Synopsis
List action scripts and their PIDs that are currently executing on the target
studio-cli vlab physical slc target list [flags]
Options
-f, --config-file string   The path of the json file that contains the DIS certificates
-g, --gwid string          The name of the gateway to establish connection
-h, --help                 help for list
-s, --target-name string   The name of the SLC target
-t, --token string         Token to authorize the request from studio cli
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc target  - Manage SLC target actions
studio-cli vlab physical slc target log
Specify an executed script PID on the gatew ay to get the output log

<!-- Page 587 -->

Synopsis
Specify an executed script PID on the gatew ay to get the output log
studio-cli vlab physical slc target log [flags]
Options
-f, --config-file string   The path of the json file that contains the DIS certificates
-w, --follow string        Optional: Follow output of the log. true|false
-g, --gwid string          The name of the gateway to establish connection
-h, --help                 help for log
-p, --log-pid string       PID of the executed script to get logs
-r, --target-name string   The name of the SLC target
-t, --token string         Token to authorize the request from studio cli
-u, --url string           The URL of the DIS server
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical slc target  - Manage SLC target actions
studio-cli vlab physical status-change
Enable or Disable physical targets
Synopsis
Enable or Disable physical targets which can be ﬁltered by id.
studio-cli vlab physical status-change [flags]
Options
-d, --disable string   Enable or Disable the target
-h, --help             help for status-change
-i, --id string        Target Id to be enabled or disabled

<!-- Page 588 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical  - Manage physical targets.
studio-cli vlab physical unreserve
Unreserv e Physical target immediately.
Synopsis
Unreserv e Physical target immediately.
studio-cli vlab physical unreserve [flags]
Options
-h, --help        help for unreserve
-i, --id string   target id
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab physical  - Manage physical targets.

<!-- Page 589 -->

studio-cli vlab property
Manage v lab properties.
Synopsis
Manage v lab properties.
Options
-h, --help        help for property
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab - Physical/V irtual target control commands
studio-cli v lab property create  - Create a v lab property
studio-cli v lab property delete  - Delete a v lab property
studio-cli v lab property list  - Get the v lab properties info
studio-cli vlab property create
Create a v lab property
Synopsis
This command creates a virtual lab property
studio-cli vlab property create [flags]
Options
-h, --help             help for create
-n, --name string      name of vlab property to list

<!-- Page 590 -->

-v, --value string     value of vlab property to list
-V, --version string   specify vlab property version to list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab property  - Manage v lab properties.
studio-cli vlab property delete
Delete a v lab property
Synopsis
This command deletes v lab property information
studio-cli vlab property delete [flags]
Options
-h, --help        help for delete
-i, --id string   Identification number of the vlab property to delete
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 591 -->

## See Also
studio-cli v lab property  - Manage v lab properties.
studio-cli vlab property list
Get the v lab properties info
Synopsis
This command lists the property information for virtual targets in v lab The command line ﬂags are
completely optional. They act as a ﬁlter to output speciﬁc data.
studio-cli vlab property list [flags]
Options
-h, --help             help for list
-i, --id string        id of vlab property to list
-n, --name string      name of vlab property to list
-v, --value string     value of vlab property to list
-V, --version string   specify vlab property version to list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab property  - Manage v lab properties.
studio-cli vlab reservation
Manage reserv ation.
Synopsis
Manage reserv ation.

<!-- Page 592 -->

Options
-h, --help        help for reservation
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab - Physical/V irtual target control commands
studio-cli v lab reserv ation list  - List all your reserv ed targets.
studio-cli v lab reserv ation modify  - Modify the virtual reserv ation vtConﬁg data ﬁelds
studio-cli vlab reservation list
List all your reserv ed targets.
Synopsis
List all your reserv ed targets.
If using the –admin argument the admin can view all the reserv ations in the system.
studio-cli vlab reservation list [flags]
Options
--admin                Use the administrator role to operate on all accounts on the
system
-g, --groups stringArray   value(s) can be either 'all' for all RBAC user groups or a
list of group names separated by commas, i.e. '<groupname1>,<groupname2>'
-h, --help                 help for list
-q, --jq string            jq query string
-t, --type string          Physical(P)|Virtual(V)|QEMU(Q)|SIMICS(S)|All(A). (default
"all")
-u, --username string      Limit scope to specific username (only works with --admin)
-v, --verbose              verbose display full reservation configuration

<!-- Page 593 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab reserv ation  - Manage reserv ation.
studio-cli vlab reservation modify
Modify the virtual reserv ation vtConﬁg data ﬁelds
Synopsis
This command modiﬁes the vtConﬁg which stands for the V irtual T arget Conﬁguration which controls
data used by the virtual target for each runtime action such as “reboot”, “reload”, etc… Y ou hav e a
choice to open an editor to look at all the av ailable arguments to modify or you can modify a single
variable with the –v ar argument.
Example for –v ar: studio-cli v lab reserv ation modify
-n 2e80f747-75ba-4a02-835f-b6152d0d8499
–var artifact_path=minio/workspace-wrlinux/builds/7/
You can use tab completion to select an av ailable target reserv ation to operate on assuming you hav e
setup tab completion.
When you do not specify –v ar an editor will open where you can change any of “v alue” sections. Y ou
can also use the –get and –put arguments to implement a separate editor sty le workﬂow.
studio-cli vlab reservation modify [flags]
Options
--admin                    Allow the use of any virtual target reservation (requires
admin role)
--get string               Save the target configuration to a file
-h, --help                     help for modify
-j, --json                     Use JSON instead of YAML for the editing
-n, --name string              Name or ID of reservation
--put string               Replace the project's local.conf with the specified file
-r, --reservation-number int   A specific reservation number ordered by earliest
reservation time
-V, --var strings              Change one or more variables in vtConfig

<!-- Page 594 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab reserv ation  - Manage reserv ation.
studio-cli vlab virtual
Manage simulation targets in virtual lab
Synopsis
Manage simulation targets in virtual lab.
Options
-h, --help        help for virtual
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 595 -->

## See Also
studio-cli v lab - Physical/V irtual target control commands
studio-cli v lab virtual create  - Create a virtual target deﬁnition using a json/y aml ﬁle (needs admin
right for v lab targets created by others).
studio-cli v lab virtual delete  - Delete virtual target deﬁnition (needs admin right for v lab targets
created by others).
studio-cli v lab virtual edit  - Edit a virtual target deﬁnition using a json/y aml ﬁle (needs admin right
for v lab targets created by others).
studio-cli v lab virtual export  - Export virtual target deﬁnition to a json template.
studio-cli v lab virtual extend  - Extend the lease on an existing reserv ation.
studio-cli v lab virtual reserv e - Reserv e a single target by id.
studio-cli v lab virtual search  - Search for targets.
studio-cli v lab virtual smoke-test  - Run a smoke test.
studio-cli v lab virtual ssh  - Manage ssh for virtual targets
studio-cli v lab virtual status  - Show the status of the reserv ation
studio-cli v lab virtual unreserv e - Unreserv e a target by reserv ation id, by name or all.
Use --group with groupnames (comma separated) to unreserve all targets in a group.
studio-cli v lab virtual update  - Update v ariables target deﬁnition attributes (needs admin right for
vlab targets created by others)
studio-cli vlab virtual create
Create a virtual target deﬁnition using a json/y aml ﬁle (needs admin right for v lab targets created by
others).
Synopsis
Create a virtual target deﬁnition using a json/y aml ﬁle (needs admin right for v lab targets created by
others).
studio-cli vlab virtual create [flags]
Options
-a, --all                 create all targets in yaml config file
-b, --branch string       git branch
-d, --dryrun              do not create target. Use with --out to debug
-f, --file string         config file for target definition [json|yaml]
-g, --git string          git repository
-i, --group-id string     specify RBAC user group id

<!-- Page 596 -->

-m, --group-name string   specify RBAC user group name
-h, --help                help for create
-n, --name string         name for the new target, or prefix for targets created with --
all
-s, --os string           operating system
-o, --out string          output JSON payload to create target to file
-p, --platform string     the simulator/emulator to create [QEMU|SIMICS]
-P, --prefix string       prefix for targets created with --all
-t, --target string       the reference target to base new target off of - for use with
yaml configs
-v, --version string      property version of sidecar
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual delete
Delete virtual target deﬁnition (needs admin right for v lab targets created by others).
Synopsis
Delete virtual target deﬁnition (needs admin right for v lab targets created by others).
studio-cli vlab virtual delete [flags]
Options
-h, --help               help for delete
-i, --id stringArray     target id(s) to delete
-n, --name stringArray   target name(s) to delete
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 597 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual edit
Edit a virtual target deﬁnition using a json/y aml ﬁle (needs admin right for v lab targets created by
others).
Synopsis
Edit a virtual target deﬁnition using a json/y aml ﬁle (needs admin right for v lab targets created by
others). If the environment v ariable ${EDITOR} is set, that editor will be launched to edit the ﬁle. By
default the output format of the ﬁle will Y AML unless using the –json argument.
You can also use the –get or –put with a ﬁle argument to implement your own type of editing
mechanism.
studio-cli vlab virtual edit [flags]
Options
--get string    Save the target configuration to a file
-h, --help          help for edit
-j, --json          Use JSON instead of YAML for the editing
-n, --name string   Name or ID of target
--put string    Replace the project's local.conf with the specified file
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 598 -->

## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual export
Export virtual target deﬁnition to a json template.
Synopsis
Export virtual target deﬁnition to a json template.
studio-cli vlab virtual export [flags]
Options
-f, --file string   write json or yaml config file for target definition
-h, --help          help for export
-i, --id string     target id to export
-n, --name string   target name to export
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual extend
Extend the lease on an existing reserv ation.
Synopsis
Extend the lease on an existing reserv ation by 1 or more hours. This can be done by reserv ation id or by
the name of target deﬁnition. If using the target deﬁnition name, all reserv ations matching the target
deﬁnition name will be extended.

<!-- Page 599 -->

If using the –admin argument, an admin can extend a reserv ation for a diﬀerent account, and it may be
combined with the –username to limit the scope to a speciﬁc account.
studio-cli vlab virtual extend [flags]
Options
--admin                    Use the administrator role to operate on all accounts on
the system
-h, --help                     help for extend
-H, --hours int                Up to 9000 hours to extend the reservation
-n, --name strings             A list of names or reservations IDs to extend the
reservation
-r, --reservation-number int   A specific reservation number ordered by earliest
reservation time
-u, --username string          Limit scope to specific username (only works with --admin)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual reserve
Reserv e a single target by id.
Synopsis
Reserv e a single target by name or id.
The default reserv ation time is for 1 hour. The –name argument takes either an id or a name.
studio-cli vlab virtual reserve [flags]

<!-- Page 600 -->

Options
-a, --auto                       Automatically reserve the target, skip adding launch
values
-c, --compute-set string         Compute set string to be passed to the backend (default
"default")
-f, --file string                file for service definition (group reserve only)
-u, --foruser string             Reserve a target for another user (requires hiveAdmin
role)
-g, --group                      Reserve a group of targets for co-simulation
-h, --help                       help for reserve
-H, --hours string               1 to 24 hours for reservation duration
-i, --id string                  target id to reserve or use --name
-n, --name string                name of target to reserve or use --id
-v, --property-version string    Sidecar property version. Run 'studio-cli vlab property
list' for list of sidecar versions
-r, --ram-resource-name string   The name of the RAM resource for user secret access
management (group access)
-p, --secret-path string         The path of user secret in vault (user access)
-s, --sshargs string             Add additional arguments to the printed ssh command
-V, --var stringArray            variable=value for target specific data
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual search
Search for targets.
Synopsis
Search for targets.
studio-cli vlab virtual search [flags]

<!-- Page 601 -->

Options
-a, --all                 Show all fields and raw data from search
--all-targets         Return all available results
--created-by string   Filter the results use createdBy
-h, --help                help for search
-i, --id string           Id of target to search for
-l, --limit int           Entries per page, 0 for all entries (default 10)
-n, --name string         Name of target to search for
-o, --offset int          Offset into search list (default 1)
--os string           os type of target to search for
-p, --platform string     [QEMU|SIMICS]
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual smoke-test
Run a smoke test.
Synopsis
Run a smoke test.
studio-cli vlab virtual smoke-test [flags]
Options
-a, --auto                       Automatically reserve the target, skip adding launch
values
-c, --compute-set string         Compute set string to be passed to the backend (default
"default")
-d, --delay string               Delay between VT commands. Valid time units are "ns",
"us" (or "µs"), "ms", "s", "m", "h" (default "5s")
-h, --help                       help for smoke-test
-i, --id string                  Target id to reserve or use --name

<!-- Page 602 -->

-n, --name string                Name of target to reserve or use --id
-r, --ram-resource-name string   The name of the RAM resource for user secret access
management (group access)
-p, --secret-path string         The path of user secret in vault (user access)
-t, --timeout string             Timeout for smoke test. Valid time units are "ns", "us"
(or "µs"), "ms", "s", "m", "h". (default "20m")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual ssh
Manage ssh for virtual targets
Synopsis
Manage ssh for virtual targets
Options
-h, --help   help for ssh
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 603 -->

## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli v lab virtual ssh key  - Manage ssh keys for virtual targets
studio-cli v lab virtual ssh user  - Manage ssh users for virtual targets
studio-cli vlab virtual ssh key
Manage ssh keys for virtual targets
Synopsis
Manage ssh keys for virtual targets
Options
-h, --help   help for key
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual ssh  - Manage ssh for virtual targets
studio-cli v lab virtual ssh key add  - Add a new SSH key to SSH Portal
studio-cli v lab virtual ssh key delete  - Delete SSH key from SSH-Portal
studio-cli v lab virtual ssh key search  - Search SSH key
studio-cli vlab virtual ssh key add
Add a new SSH key to SSH Portal

<!-- Page 604 -->

Synopsis
Add a new SSH key for a user.
This command allows admins to create SSH keys for any user by specifying the username.
Regular users can add SSH keys only for themselves. It requires the path to the SSH public
key file to be provided.
studio-cli vlab virtual ssh key add --sshKeyFile ~/.ssh/id_rsa.pub
studio-cli vlab virtual ssh key add --sshKeyFile ~/.ssh/id_rsa.pub --username testuser
(must be admin)
studio-cli vlab virtual ssh key add [flags]
Options
-h, --help                help for add
-f, --sshKeyFile string   path to ssh key as an example ~/.ssh/id_rsa.pub
-u, --username string     username for the new user creation (must be admin)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual ssh key  - Manage ssh keys for virtual targets
studio-cli vlab virtual ssh key delete
Delete SSH key from SSH-Portal
Synopsis
Deletes an SSH key from the system. This command requires the --id flag to specify the
unique identifier of the SSH key to be deleted.
It ensures secure and precise management of SSH keys by enforcing role-based restrictions:
Admins can delete any user's SSH key.
Regular users can only delete SSH keys registered under their own username.
studio-cli vlab virtual ssh key delete [flags]

<!-- Page 605 -->

Options
-h, --help        help for delete
-i, --id string   id of ssh-key
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual ssh key  - Manage ssh keys for virtual targets
studio-cli vlab virtual ssh key search
Search SSH key
Synopsis
Allows users to retrieve their own SSH key data.
Admin users can perform a search for SSH keys by specifying a username. This command is
useful for managing and retrieving SSH access details.
studio-cli vlab ssh user search
studio-cli vlab ssh user search -n testuser (must be admin)
studio-cli vlab virtual ssh key search [flags]
Options
-h, --help              help for search
-i, --id string         key id
-u, --username string   username for the new user creation (must be admin)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 606 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual ssh key  - Manage ssh keys for virtual targets
studio-cli vlab virtual ssh user
Manage ssh users for virtual targets
Synopsis
Manage ssh users for virtual targets
Options
-h, --help   help for user
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual ssh  - Manage ssh for virtual targets
studio-cli v lab virtual ssh user create  - Create SSH user
studio-cli v lab virtual ssh user delete  - Delete SSH user
studio-cli v lab virtual ssh user search  - Search SSH user
studio-cli vlab virtual ssh user create
Create SSH user

<!-- Page 607 -->

Synopsis
Creates a new SSH user. Regular users can only create their own account, with data derived
from their JWT token. Admin users can create new accounts for others by specifying the
username and email using the appropriate flags.
studio-cli vlab virtual ssh user create
studio-cli vlab virtual ssh user create --username testuser --email testuser@example.com
(must be amdin)
studio-cli vlab virtual ssh user create [flags]
Options
-e, --email string      user email (must be amdin)
-h, --help              help for create
-u, --username string   username for the new user creation (must be amdin)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual ssh user  - Manage ssh users for virtual targets
studio-cli vlab virtual ssh user delete
Delete SSH user
Synopsis
Deletes an SSH user. Regular users can only delete their own account, while admins can
delete any user's account by specifying the username with the --username flag.
studio-cli vlab virtual ssh user delete
studio-cli vlab virtual ssh user delete --username
studio-cli vlab virtual ssh user delete [flags]

<!-- Page 608 -->

Options
-h, --help              help for delete
-u, --username string   username for the new user creation (must be admin)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual ssh user  - Manage ssh users for virtual targets
studio-cli vlab virtual ssh user search
Search SSH user
Synopsis
Retriev e SSH user own data, admins can search any by username
studio-cli vlab ssh user search -n testuser
studio-cli vlab virtual ssh user search [flags]
Options
-h, --help              help for search
-u, --username string   username for the new user creation --admin required
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 609 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual ssh user  - Manage ssh users for virtual targets
studio-cli vlab virtual status
Show the status of the reserv ation
Synopsis
This command prints the status of a speciﬁc reserv ation, or reports the status of all reserv ations when
no reserv ation id is provided.
This command reports back directly from kubernetes so you can ﬁnd out when the reserv ed pod has
launched, or why it failed to launch.
studio-cli vlab virtual status [flags]
Options
-h, --help          help for status
-i, --id string     ID of reservation
-n, --name string   Name of target
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual unreserve
Unreserv e a target by reserv ation id, by name or all.

<!-- Page 610 -->

Use --group with groupnames (comma separated) to unreserve all targets in a group.
Synopsis
Unreserv e a target by reserv ation id, by name or all.
Use –group with groupnames (comma separated) to unreserv e all targets in a group.
If using the –admin argument you must also provide a username, in which case you can end a
reserv ation of another username.
studio-cli vlab virtual unreserve [flags]
Options
--admin                    Use the administrator role to operate on all accounts on
the system
-a, --all                      unreserve all your targets
-g, --group strings            unreserve all targets in a group. Option requires group
name or names (comma separated).
-h, --help                     help for unreserve
-i, --id strings               unreserve one or multiple targets by ids separated by ',',
or providing flag multiple times
-n, --name strings             unreserve one or multiple targets by names separated by
',', or providing flag multiple times
-r, --reservation-number int   A specific reservation number ordered by earliest
reservation time
-u, --username string          Limit scope to specific username (only works with --admin)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vlab virtual update
Update v ariables target deﬁnition attributes (needs admin right for v lab targets created by others)

<!-- Page 611 -->

Synopsis
You can toggle a target deﬁnition betw een –public and –priv ate.
You must hav e a virtual lab administrator role in order to update others v lab targets.
You can also update individual data ﬁelds of the target deﬁnition with one or more –v ar arguments.
Example: studio-cli v lab virtual \ update -n MY_T arget –v ar=artifact_path.default=minio\build\123
It is often easier to use the “studio-cli v lab virtual edit” command instead of using individual –v ar
arguments to modify a target deﬁnition.
studio-cli vlab virtual update [flags]
Options
-h, --help              help for update
-i, --id string         Target id
-n, --name string       Name of target
--private           Make the target definition private
--public            Make the target definition public
-V, --var stringArray   Update variable.value=data for target specific data
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli v lab virtual  - Manage simulation targets in virtual lab
studio-cli vxbs
VxWorks Build System commands
Synopsis
VxWorks Build System Commands

<!-- Page 612 -->

Options
-h, --help        help for vxbs
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli vxbs artifact  - Artifact commands.
studio-cli vxbs build  - Conﬁg build
studio-cli vxbs conﬁguration  - List av ailable artifacts/board/Cpu/release
studio-cli vxbs dashboard  - Show my dashboard
studio-cli vxbs group  - Group commands
studio-cli vxbs project  - Project commands
studio-cli vxbs vip  - Conﬁg VIP
studio-cli vxbs vsb  - Conﬁg VSB
studio-cli vxbs artifact
Artifact commands.
Synopsis
Artifact commands.
Options
-h, --help        help for artifact
-q, --jq string   jq query string override

<!-- Page 613 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs  - VxWorks Build System commands
studio-cli vxbs artifact get  - Get the artifact details.
studio-cli vxbs artifact list  - List av ailable artifacts
studio-cli vxbs artifact list-all  - Get an artifact list
studio-cli vxbs artifact get
Get the artifact details.
Synopsis
Get the artifact details with its metadata and conﬁguration.
studio-cli vxbs artifact get [flags]
Options
-b, --build-id string   Uuid of the build task
-h, --help              help for get
-i, --id string         Uuid of the artifact
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 614 -->

## See Also
studio-cli vxbs artifact  - Artifact commands.
studio-cli vxbs artifact list
List av ailable artifacts
Synopsis
List av ailable artifacts which hav e been built by you. By default only the VSB type objects are listed.
You can add “–jq .” to view all the artifacts not created by you.
studio-cli vxbs artifact list [flags]
Options
-h, --help             help for list
-r, --release string   Release id
-t, --type string      Artifact type (default is VSB when not specified) (default "VSB")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs artifact  - Artifact commands.
studio-cli vxbs artifact list-all
Get an artifact list
Synopsis
Get an artifact list

<!-- Page 615 -->

studio-cli vxbs artifact list-all [flags]
Options
-h, --help   help for list-all
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string override
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs artifact  - Artifact commands.
studio-cli vxbs build
Conﬁg build
Synopsis
Conﬁg build
Options
-d, --description string   script description
-h, --help                 help for build
-i, --id string            Project id to do config
-n, --name string          Project name to do config
--script string        script url
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]

<!-- Page 616 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs  - VxWorks Build System commands
studio-cli vxbs build cancel  - Cancel build
studio-cli vxbs build get  - Get build details
studio-cli vxbs build list-activ e - Get activ e builds
studio-cli vxbs build list-completed  - Get completed builds
studio-cli vxbs build postbuild  - Manager post build for projects
studio-cli vxbs build prebuild  - Manager prebuild for projects
studio-cli vxbs build start  - Build start
studio-cli vxbs build y amlﬁle  - Manager y aml ﬁle for projects
studio-cli vxbs build cancel
Cancel build
Synopsis
Cancel build
studio-cli vxbs build cancel [flags]
Options
-b, --buildid string   Cancel by build id
-h, --help             help for cancel
-i, --id string        Cancel all builds for project id
-n, --name string      Cancel all builds for project name
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-q, --jq string            jq query string
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes

<!-- Page 617 -->

--script string        script url
--totpcode string      TOTP code
## See Also
studio-cli vxbs build  - Conﬁg build
studio-cli vxbs build get
Get build details
Synopsis
Get build details
studio-cli vxbs build get [flags]
Options
-h, --help        help for get
-i, --id string   Uuid of the build task
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--script string        script url
--totpcode string      TOTP code
## See Also
studio-cli vxbs build  - Conﬁg build
studio-cli vxbs build list-active
Get activ e builds

<!-- Page 618 -->

Synopsis
Get activ e builds
studio-cli vxbs build list-active [flags]
Options
-b, --builduuid strings   Check active status on specific Build UUID's or wait on them
-h, --help                help for list-active
-i, --id string           List builds by this project id
-l, --limit int           Limit project list to X entries per page(0 for all) (default
10)
-n, --name string         List builds by this project name
--nofilter            Do not use the task filter for logging
-p, --page int            Page number of listing when using a limit (default 1)
--quiet               Do not use the task filter for logging
-s, --sleep int           When waiting for the build(s) to complete do not emit status
(default 5)
-w, --wait                Wait for the build(s) to complete
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-q, --jq string            jq query string
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--script string        script url
--totpcode string      TOTP code
## See Also
studio-cli vxbs build  - Conﬁg build
studio-cli vxbs build list-completed
Get completed builds
Synopsis
Get completed builds
studio-cli vxbs build list-completed [flags]

<!-- Page 619 -->

Options
-h, --help          help for list-completed
-i, --id string     List builds by this project id
-l, --limit int     Limit project list to X entries per page (0 for all) (default 10)
-n, --name string   List builds by this project name
-p, --page int      Page number of listing when using a limit (default 1)
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-q, --jq string            jq query string
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--script string        script url
--totpcode string      TOTP code
## See Also
studio-cli vxbs build  - Conﬁg build
studio-cli vxbs build postbuild
Manager post build for projects
Synopsis
Manager post build for projects
Options
-h, --help   help for postbuild
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config

<!-- Page 620 -->

--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--script string        script url
--totpcode string      TOTP code
## See Also
studio-cli vxbs build  - Conﬁg build
studio-cli vxbs build postbuild add  - Add postbuild
studio-cli vxbs build postbuild disable  - Disable postbuild
studio-cli vxbs build postbuild enable  - Enable postbuild
studio-cli vxbs build postbuild remov e - Remov e postbuild
studio-cli vxbs build postbuild add
Add postbuild
Synopsis
Add postbuild
studio-cli vxbs build postbuild add [flags]
Options
-h, --help            help for add
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code

<!-- Page 621 -->

## See Also
studio-cli vxbs build postbuild  - Manager post build for projects
studio-cli vxbs build postbuild disable
Disable postbuild
Synopsis
Disable postbuild
studio-cli vxbs build postbuild disable [flags]
Options
-h, --help            help for disable
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build postbuild  - Manager post build for projects
studio-cli vxbs build postbuild enable
Enable postbuild
Synopsis
Enable postbuild

<!-- Page 622 -->

studio-cli vxbs build postbuild enable [flags]
Options
-h, --help            help for enable
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build postbuild  - Manager post build for projects
studio-cli vxbs build postbuild remove
Remov e postbuild
Synopsis
Remov e postbuild
studio-cli vxbs build postbuild remove [flags]
Options
-h, --help            help for remove
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.

<!-- Page 623 -->

-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build postbuild  - Manager post build for projects
studio-cli vxbs build prebuild
Manager prebuild for projects
Synopsis
Manager prebuild for projects
Options
-h, --help   help for prebuild
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--script string        script url
--totpcode string      TOTP code

<!-- Page 624 -->

## See Also
studio-cli vxbs build  - Conﬁg build
studio-cli vxbs build prebuild add  - Add prebuild
studio-cli vxbs build prebuild disable  - Disable prebuild
studio-cli vxbs build prebuild enable  - Enable prebuild
studio-cli vxbs build prebuild remov e - Remov e prebuild
studio-cli vxbs build prebuild add
Add prebuild
Synopsis
Add prebuild
studio-cli vxbs build prebuild add [flags]
Options
-h, --help            help for add
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build prebuild  - Manager prebuild for projects

<!-- Page 625 -->

studio-cli vxbs build prebuild disable
Disable prebuild
Synopsis
Disable prebuild
studio-cli vxbs build prebuild disable [flags]
Options
-h, --help            help for disable
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build prebuild  - Manager prebuild for projects
studio-cli vxbs build prebuild enable
Enable prebuild
Synopsis
Enable prebuild
studio-cli vxbs build prebuild enable [flags]
Options
-h, --help            help for enable

<!-- Page 626 -->

-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build prebuild  - Manager prebuild for projects
studio-cli vxbs build prebuild remove
Remov e prebuild
Synopsis
Remov e prebuild
studio-cli vxbs build prebuild remove [flags]
Options
-h, --help            help for remove
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]

<!-- Page 627 -->

--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build prebuild  - Manager prebuild for projects
studio-cli vxbs build start
Build start
Synopsis
Build start
studio-cli vxbs build start [flags]
Options
--ade                    Build ade (Application Development Environment) - allows to
develop applications with Workbench - only available for VIP projects
-a, --artifact-path string   Artifact path
-h, --help                   help for start
-i, --id string              Project id
-n, --name string            Project name
--nofilter               Do not use the task filter for logging
--quiet                  When waiting for the build(s) to complete do not emit status
--sdk                    Build sdk, only available when project is VIP
-s, --sleep int              Sleep poll interval in seconds (default: 5) (default 5)
-w, --wait                   Wait for the build(s) to complete
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-q, --jq string            jq query string
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--script string        script url
--totpcode string      TOTP code
## See Also
studio-cli vxbs build  - Conﬁg build

<!-- Page 628 -->

studio-cli vxbs build yamlfile
Manager y aml ﬁle for projects
Synopsis
Manager y aml ﬁle for projects
Options
-h, --help   help for yamlfile
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--script string        script url
--totpcode string      TOTP code
## See Also
studio-cli vxbs build  - Conﬁg build
studio-cli vxbs build y amlﬁle add  - Add customized y aml ﬁle to conﬁg build
studio-cli vxbs build y amlﬁle disable  - Disable customized y aml ﬁle to conﬁg build
studio-cli vxbs build y amlﬁle enable  - Enable customized y aml ﬁle to conﬁg build
studio-cli vxbs build y amlﬁle remov e - Remov e customized y aml ﬁle
studio-cli vxbs build yamlfile add
Add customized y aml ﬁle to conﬁg build
Synopsis
Add customized y aml ﬁle to conﬁg build
studio-cli vxbs build yamlfile add [flags]

<!-- Page 629 -->

Options
-h, --help            help for add
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build y amlﬁle  - Manager y aml ﬁle for projects
studio-cli vxbs build yamlfile disable
Disable customized y aml ﬁle to conﬁg build
Synopsis
Disable customized y aml ﬁle to conﬁg build
studio-cli vxbs build yamlfile disable [flags]
Options
-h, --help            help for disable
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string

<!-- Page 630 -->

-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build y amlﬁle  - Manager y aml ﬁle for projects
studio-cli vxbs build yamlfile enable
Enable customized y aml ﬁle to conﬁg build
Synopsis
Enable customized y aml ﬁle to conﬁg build
studio-cli vxbs build yamlfile enable [flags]
Options
-h, --help            help for enable
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build y amlﬁle  - Manager y aml ﬁle for projects

<!-- Page 631 -->

studio-cli vxbs build yamlfile remove
Remov e customized y aml ﬁle
Synopsis
Remov e customized y aml ﬁle
studio-cli vxbs build yamlfile remove [flags]
Options
-h, --help            help for remove
-s, --script string   script url
Options inherited from parent commands
--debughttp            Print information for http transactions and timings
--debughttp2           Print extended debug data for http requests
--debughttp3           Print extended debug data for http responses
--debughttp4           Pretty print debug data for http responses and requests.
-d, --description string   script description
-i, --id string            Project id to do config
-q, --jq string            jq query string
-n, --name string          Project name to do config
--non-interactive      Disable all interactive mode
--output               Set Output Format: [json|yaml]
--raw                  Strip single result jq queries of quotes
--totpcode string      TOTP code
## See Also
studio-cli vxbs build y amlﬁle  - Manager y aml ﬁle for projects
studio-cli vxbs configuration
List av ailable artifacts/board/Cpu/release
Synopsis
List av ailable artifacts/board/Cpu/release.
Options
-h, --help   help for configuration

<!-- Page 632 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs  - VxWorks Build System commands
studio-cli vxbs conﬁguration list-board  - List av ailable boards for the selected release
studio-cli vxbs conﬁguration list-cpu  - List av ailable cpus for the selected release
studio-cli vxbs conﬁguration list-release  - List av ailable releases
studio-cli vxbs conﬁguration setup-access-conﬁg  - Conﬁgure the access conﬁguration for XBS with
necessary resources
studio-cli vxbs conﬁguration setup-existing-access-conﬁg  - Conﬁgure and existing access
conﬁguration for VXBS with existing resources
studio-cli vxbs conﬁguration teardown-access-conﬁg  - Teardown the access conﬁguration and
dependent resources
studio-cli vxbs configuration list-board
List av ailable boards for the selected release
Synopsis
List all the boards av ailable for the selected release. Y ou can add –raw to remov e the quotes.
studio-cli vxbs configuration list-board [flags]
Options
-a, --access-config string   Access config name
-b, --board string           VSB board
-h, --help                   help for list-board
-r, --release string         Release id

<!-- Page 633 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs conﬁguration  - List av ailable artifacts/board/Cpu/release
studio-cli vxbs configuration list-cpu
List av ailable cpus for the selected release
Synopsis
List all the cpus av ailable for the selected release. Y ou can add –raw to remov e the quotes.
studio-cli vxbs configuration list-cpu [flags]
Options
-a, --access-config string   Access config name
-h, --help                   help for list-cpu
-r, --release string         Release id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs conﬁguration  - List av ailable artifacts/board/Cpu/release

<!-- Page 634 -->

studio-cli vxbs configuration list-release
List av ailable releases
Synopsis
List the av ailable releases. Y ou can add –raw to remov e the quotes.
studio-cli vxbs configuration list-release [flags]
Options
-a, --access-config string   Access config name
-h, --help                   help for list-release
-r, --release string         Release id
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs conﬁguration  - List av ailable artifacts/board/Cpu/release
studio-cli vxbs configuration setup-access-config
Conﬁgure the access conﬁguration for XBS with necessary resources
Synopsis
Conﬁgure the access conﬁguration for XBS with necessary resources
studio-cli vxbs configuration setup-access-config [flags]
Options
-a, --artifacts-bucket string   Artifacts bucket, needs to be passed in the format of
workspace-<bucket-name>
-g, --gitlab-group string       Gitlab group
-h, --help                      help for setup-access-config

<!-- Page 635 -->

-n, --name string               Access config name
-r, --releases string           Releases
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs conﬁguration  - List av ailable artifacts/board/Cpu/release
studio-cli vxbs configuration setup-existing-access-config
Conﬁgure and existing access conﬁguration for VXBS with existing resources
Synopsis
Conﬁgure and existing access conﬁguration for VXBS with existing resources, it creates the access
conﬁguration if it doesn’t exist
studio-cli vxbs configuration setup-existing-access-config [flags]
Options
-a, --artifacts-bucket string   Artifacts bucket (comma separated, each item should be in
the format of workspace-<bucket-name>)
-g, --gitlab-group string       Gitlab group (comma separated)
-h, --help                      help for setup-existing-access-config
-n, --name string               Access config name, if it doesn't exist it will be
created
-r, --releases string           Releases (comma separated)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode

<!-- Page 636 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs conﬁguration  - List av ailable artifacts/board/Cpu/release
studio-cli vxbs configuration teardown-access-config
Teardown the access conﬁguration and dependent resources
Synopsis
Teardown the access conﬁguration and dependent resources
studio-cli vxbs configuration teardown-access-config [flags]
Options
-a, --artifacts-bucket string   Artifacts bucket (comma separated, each item should be in
the format of workspace-<bucket-name>)
-g, --gitlab-group string       Gitlab group (comma separated)
-h, --help                      help for teardown-access-config
-n, --name string               Access config name
-r, --releases string           Releases
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs conﬁguration  - List av ailable artifacts/board/Cpu/release
studio-cli vxbs dashboard
Show my dashboard

<!-- Page 637 -->

Synopsis
Show my dashboard
studio-cli vxbs dashboard [flags]
Options
-h, --help          help for dashboard
-l, --limit int     Limit project list to X entries per page (0 for all) (default 10)
-n, --name string   List projects matching partial name
-p, --page int      Page number of listing when using a limit (default 1)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs  - VxWorks Build System commands
studio-cli vxbs group
Group commands
Synopsis
Run a project group sub command
Options
-h, --help   help for group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.

<!-- Page 638 -->

-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs  - VxWorks Build System commands
studio-cli vxbs group assign  - Assign group access
studio-cli vxbs group revoke  - Revoke group access
studio-cli vxbs group assign
Assign group access
Synopsis
Assign a group access
studio-cli vxbs group assign [flags]
Options
-g, --group string      group name
-h, --help              help for assign
-i, --id string         Project id
-n, --name string       Name of project
-r, --rolename string   Role (viewer, tester, editor, or lead) (default "viewer")
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs group  - Group commands

<!-- Page 639 -->

studio-cli vxbs group revoke
Revoke group access
Synopsis
Revoke a group access
studio-cli vxbs group revoke [flags]
Options
-g, --group string   project privacy revoke user name.
-h, --help           help for revoke
-i, --id string      Project id
-n, --name string    Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs group  - Group commands
studio-cli vxbs project
Project commands
Synopsis
Run a project sub command, or in the absence of a sub command display the project details.
Options
-h, --help          help for project
-i, --id string     VSB Project id to do config
-n, --name string   VSB Project name to do config

<!-- Page 640 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs  - VxWorks Build System commands
studio-cli vxbs project archiv e - Mov e project to archiv e list
studio-cli vxbs project assign-access-conﬁg  - Assign a project to an access conﬁg
studio-cli vxbs project clone  - Clone a project.
studio-cli vxbs project close  - Close project
studio-cli vxbs project get  - Project information.
studio-cli vxbs project list  - Search through projects
studio-cli vxbs project list-archiv ed - Get the list of archiv ed projects
studio-cli vxbs project remov e - Remov e project
studio-cli vxbs project restore  - Restore archiv ed project
studio-cli vxbs project sav e - Sav e conﬁgs
studio-cli vxbs project archive
Mov e project to archiv e list
Synopsis
Mov e project to archiv e list
studio-cli vxbs project archive [flags]
Options
-h, --help          help for archive
-i, --id string     Project id
-n, --name string   Name of project

<!-- Page 641 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands
studio-cli vxbs project assign-access-config
Assign a project to an access conﬁg
Synopsis
Assign a project to an access conﬁg
studio-cli vxbs project assign-access-config [flags]
Options
-a, --access-config string   access-config name
-h, --help                   help for assign-access-config
-n, --name string            project name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VSB Project id to do config
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands

<!-- Page 642 -->

studio-cli vxbs project clone
Clone a project.
Synopsis
Clone a project.
studio-cli vxbs project clone [flags]
Options
-a, --access string               Sharing access level, LEAD|EDITOR|TESTER|VIEWER
(default "LEAD")
-f, --access-config-name string   Access config name
-c, --cname string                The existing project name
-p, --gitlab-group-name string    Gitlab group name
-g, --groupname string            Group name
-h, --help                        help for clone
-i, --id string                   The existing project id
-n, --name string                 Name of the new project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands
studio-cli vxbs project close
Close project
Synopsis
Close project

<!-- Page 643 -->

studio-cli vxbs project close [flags]
Options
-h, --help          help for close
-i, --id string     Project id
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands
studio-cli vxbs project get
Project information.
Synopsis
Project information.
studio-cli vxbs project get [flags]
Options
-h, --help          help for get
-i, --id string     Project id
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string

<!-- Page 644 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands
studio-cli vxbs project list
Search through projects
Synopsis
Search through projects
studio-cli vxbs project list [flags]
Options
-a, --archive       List archive projects
-h, --help          help for list
-l, --limit int     Limit project list to X entries per page (0 for all) (default 10)
-n, --name string   List projects matching partial name
-p, --page int      Page number of listing when using a limit (default 1)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VSB Project id to do config
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands

<!-- Page 645 -->

studio-cli vxbs project list-archived
Get the list of archiv ed projects
Synopsis
Get the list of archiv ed projects
studio-cli vxbs project list-archived [flags]
Options
-h, --help   help for list-archived
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VSB Project id to do config
-q, --jq string         jq query string
-n, --name string       VSB Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands
studio-cli vxbs project remove
Remov e project
Synopsis
Remov e project
studio-cli vxbs project remove [flags]
Options
-h, --help          help for remove
-i, --id string     Project id
-n, --name string   Name of project

<!-- Page 646 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands
studio-cli vxbs project restore
Restore archiv ed project
Synopsis
Restore archiv ed project
studio-cli vxbs project restore [flags]
Options
-h, --help          help for restore
-i, --id string     Project id
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands

<!-- Page 647 -->

studio-cli vxbs project save
Save conﬁgs
Synopsis
Save conﬁgs
studio-cli vxbs project save [flags]
Options
-h, --help          help for save
-i, --id string     Project id
-n, --name string   Name of project
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs project  - Project commands
studio-cli vxbs vip
Conﬁg VIP
Synopsis
Conﬁg VIP
Options
-h, --help          help for vip
-i, --id string     VIP Project id to do config
-n, --name string   VIP Project name to do config

<!-- Page 648 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs  - VxWorks Build System commands
studio-cli vxbs vip bundle  - Conﬁg VIP bundle
studio-cli vxbs vip component  - Conﬁg VIP components
studio-cli vxbs vip create  - Vip create
studio-cli vxbs vip ﬁle  - Conﬁg VIP ﬁle
studio-cli vxbs vip lkm  - Linkable Kernel Module
studio-cli vxbs vip parameter  - Conﬁg VIP parameter
studio-cli vxbs vip romfs  - Conﬁg VIP romfs
studio-cli vxbs vip bundle
Conﬁg VIP bundle
Synopsis
Conﬁg VIP bundle
Options
-h, --help   help for bundle
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode

<!-- Page 649 -->

--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip  - Conﬁg VIP
studio-cli vxbs vip bundle add  - Add bundles
studio-cli vxbs vip bundle remov e - Remov e bundles
studio-cli vxbs vip bundle add
Add bundles
Synopsis
Add one or more bundles separated by a space
studio-cli vxbs vip bundle add [flags]
Options
-h, --help   help for add
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip bundle  - Conﬁg VIP bundle

<!-- Page 650 -->

studio-cli vxbs vip bundle remove
Remov e bundles
Synopsis
Remov e one or more bundles separated by a space
studio-cli vxbs vip bundle remove [flags]
Options
-h, --help   help for remove
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip bundle  - Conﬁg VIP bundle
studio-cli vxbs vip component
Conﬁg VIP components
Synopsis
Conﬁg VIP components
Options
-h, --help   help for component

<!-- Page 651 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip  - Conﬁg VIP
studio-cli vxbs vip component add  - Add component
studio-cli vxbs vip component remov e - Remov e components
studio-cli vxbs vip component add
Add component
Synopsis
Add one or more components separated by a space.
studio-cli vxbs vip component add [flags]
Options
-h, --help   help for add
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 652 -->

## See Also
studio-cli vxbs vip component  - Conﬁg VIP components
studio-cli vxbs vip component remove
Remov e components
Synopsis
Remov e one or more components using one or more -c ﬂags.
studio-cli vxbs vip component remove [flags]
Options
-h, --help   help for remove
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip component  - Conﬁg VIP components
studio-cli vxbs vip create
Vip create
Synopsis
Vip create
studio-cli vxbs vip create [flags]

<!-- Page 653 -->

Options
-a, --access string               sharing access level, LEAD|EDITOR|TESTER|VIEWER
(default "LEAD")
-f, --access-config-name string   Access config name
--board string                VIP board
--debug                       VSB debug mode
--debug-opt                   VSB debug mode
-d, --desc string                 Optional description for project
-p, --gitlab-group-name string    Gitlab group name
-g, --groupname string            group name
-h, --help                        help for create
-n, --name string                 VIP Project name
--profile string              VIP profile
PROFILE_BOOTAPP|PROFILE_DEVELOPMENT|PROFILE_HARDENED|PROFILE_INTEL_GENERIC|PROFILE_MINIMAL|
PROFILE_MINIMAL_RTP|PROFILE_STANDALONE_DEVELOPMENT|PROFILE_STANDALONE_MINIMAL|PROFILE_STAND
## Alone_Minimal_Rtp
-r, --release string              Release id
-t, --type string                 'private' or 'public' or 'shared' role type, default to
'public' (default "public")
-b, --vsb string                  VIP base on VSB (or use the build number)
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip  - Conﬁg VIP
studio-cli vxbs vip file
Conﬁg VIP ﬁle
Synopsis
Conﬁg VIP ﬁle

<!-- Page 654 -->

Options
-h, --help   help for file
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip  - Conﬁg VIP
studio-cli vxbs vip ﬁle get  - Get a ﬁle from VIP
studio-cli vxbs vip ﬁle list  - List VIP ﬁles
studio-cli vxbs vip ﬁle post  - Adds a new ﬁle to a GitLab repository.
studio-cli vxbs vip ﬁle put  - Put a ﬁle to VIP
studio-cli vxbs vip file get
Get a ﬁle from VIP
Synopsis
Get a ﬁle from VIP
studio-cli vxbs vip file get [flags]
Options
-f, --file string            Output file name
-h, --help                   help for get
-r, --resource-name string   Name of file resource

<!-- Page 655 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip ﬁle  - Conﬁg VIP ﬁle
studio-cli vxbs vip file list
List VIP ﬁles
Synopsis
List VIP ﬁles
studio-cli vxbs vip file list [flags]
Options
-h, --help   help for list
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 656 -->

## See Also
studio-cli vxbs vip ﬁle  - Conﬁg VIP ﬁle
studio-cli vxbs vip file post
Adds a new ﬁle to a GitLab repository.
Synopsis
Adds a new ﬁle to a GitLab repository..
studio-cli vxbs vip file post [flags]
Options
-f, --folder-name string   Folder name of the file,such as 'Kernel Application Files'.
-u, --gitlab-url string    URL from GitLab
-h, --help                 help for post
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip ﬁle  - Conﬁg VIP ﬁle
studio-cli vxbs vip file put
Put a ﬁle to VIP
Synopsis
Put a ﬁle to VIP

<!-- Page 657 -->

studio-cli vxbs vip file put [flags]
Options
-f, --file string            Inputfile name
-h, --help                   help for put
-r, --resource-name string   Name of file resource
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip ﬁle  - Conﬁg VIP ﬁle
studio-cli vxbs vip lkm
Linkable Kernel Module
Synopsis
LKM (Linkable Kernel Module) is a DKM (Downloadable Kernel Module) that’s statically linked into
the kernel image
Options
-h, --help   help for lkm
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string

<!-- Page 658 -->

-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip  - Conﬁg VIP
studio-cli vxbs vip lkm add  - Add lkm to VIP
studio-cli vxbs vip lkm remov e - Remov e lkm from VIP
studio-cli vxbs vip lkm add
Add lkm to VIP
Synopsis
Add lkm to VIP
studio-cli vxbs vip lkm add [flags]
Options
--gitlab string   gitlab url when source is application
-h, --help            help for add
--path string     the file path
--ptype string    project type when source is application
--source string   the file come from VSB|Gitlab|Artifacts|Application
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip lkm  - Linkable Kernel Module

<!-- Page 659 -->

studio-cli vxbs vip lkm remove
Remov e lkm from VIP
Synopsis
Remov e lkm from VIP
studio-cli vxbs vip lkm remove [flags]
Options
--fname string   file name
-h, --help           help for remove
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip lkm  - Linkable Kernel Module
studio-cli vxbs vip parameter
Conﬁg VIP parameter
Synopsis
Conﬁg VIP parameter
Options
-h, --help               help for parameter
-p, --parameter string   parameter name

<!-- Page 660 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip  - Conﬁg VIP
studio-cli vxbs vip parameter get  - Get VIP parameters
studio-cli vxbs vip parameter set  - Set VIP parameters
studio-cli vxbs vip parameter get
Get VIP parameters
Synopsis
Get VIP parameters
studio-cli vxbs vip parameter get [flags]
Options
-h, --help   help for get
Options inherited from parent commands
--debughttp          Print information for http transactions and timings
--debughttp2         Print extended debug data for http requests
--debughttp3         Print extended debug data for http responses
--debughttp4         Pretty print debug data for http responses and requests.
-i, --id string          VIP Project id to do config
-q, --jq string          jq query string
-n, --name string        VIP Project name to do config
--non-interactive    Disable all interactive mode
--output             Set Output Format: [json|yaml]
-p, --parameter string   parameter name

<!-- Page 661 -->

--raw                Strip single result jq queries of quotes
--totpcode string    TOTP code
## See Also
studio-cli vxbs vip parameter  - Conﬁg VIP parameter
studio-cli vxbs vip parameter set
Set VIP parameters
Synopsis
Set VIP parameters
studio-cli vxbs vip parameter set [flags]
Options
-h, --help           help for set
-v, --value string   parameter value
Options inherited from parent commands
--debughttp          Print information for http transactions and timings
--debughttp2         Print extended debug data for http requests
--debughttp3         Print extended debug data for http responses
--debughttp4         Pretty print debug data for http responses and requests.
-i, --id string          VIP Project id to do config
-q, --jq string          jq query string
-n, --name string        VIP Project name to do config
--non-interactive    Disable all interactive mode
--output             Set Output Format: [json|yaml]
-p, --parameter string   parameter name
--raw                Strip single result jq queries of quotes
--totpcode string    TOTP code
## See Also
studio-cli vxbs vip parameter  - Conﬁg VIP parameter
studio-cli vxbs vip romfs
Conﬁg VIP romfs

<!-- Page 662 -->

Synopsis
Conﬁg VIP romfs
Options
-h, --help           help for romfs
--pname string   parent folder name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip  - Conﬁg VIP
studio-cli vxbs vip romfs ﬁle  - Add ﬁle to VIP ROMFS
studio-cli vxbs vip romfs folder  - Add folder to VIP ROMFS
studio-cli vxbs vip romfs remov e - Remov e ﬁle/folder from VIP ROMFS
studio-cli vxbs vip romfs file
Add ﬁle to VIP ROMFS
Synopsis
Add ﬁle to VIP ROMFS
Options
-h, --help   help for file
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 663 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--pname string      parent folder name
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip romfs  - Conﬁg VIP romfs
studio-cli vxbs vip romfs ﬁle add  - Add ﬁle to ROMFS
studio-cli vxbs vip romfs file add
Add ﬁle to ROMFS
Synopsis
Add ﬁle to ROMFS
studio-cli vxbs vip romfs file add [flags]
Options
--atype string    application type when source is application
--gitlab string   gitlab url when source is application
-h, --help            help for add
--path string     the file path
--ptype string    project type when source is application
--source          the file come from VSB|Gitlab|Artifacts|Application
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--pname string      parent folder name

<!-- Page 664 -->

--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip romfs ﬁle  - Add ﬁle to VIP ROMFS
studio-cli vxbs vip romfs folder
Add folder to VIP ROMFS
Synopsis
Add folder to VIP ROMFS
Options
-h, --help   help for folder
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--pname string      parent folder name
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip romfs  - Conﬁg VIP romfs
studio-cli vxbs vip romfs folder add  - Add folder to ROMFS
studio-cli vxbs vip romfs folder add
Add folder to ROMFS

<!-- Page 665 -->

Synopsis
Add folder to ROMFS
studio-cli vxbs vip romfs folder add [flags]
Options
--fname string   folder name
-h, --help           help for add
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--pname string      parent folder name
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip romfs folder  - Add folder to VIP ROMFS
studio-cli vxbs vip romfs remove
Remov e ﬁle/folder from VIP ROMFS
Synopsis
Remov e ﬁle/folder from VIP ROMFS
studio-cli vxbs vip romfs remove [flags]
Options
--fname string   folder or file name
-h, --help           help for remove

<!-- Page 666 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VIP Project id to do config
-q, --jq string         jq query string
-n, --name string       VIP Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--pname string      parent folder name
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vip romfs  - Conﬁg VIP romfs
studio-cli vxbs vsb
Conﬁg VSB
Synopsis
Conﬁg VSB
Options
-h, --help          help for vsb
-i, --id string     VSB Project id to do config
-n, --name string   VSB Project name to do config
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 667 -->

## See Also
studio-cli vxbs  - VxWorks Build System commands
studio-cli vxbs vsb add  - Add VSB lay er
studio-cli vxbs vsb create  - Vsb create
studio-cli vxbs vsb disable  - Disable VSB lay er
studio-cli vxbs vsb enable  - Enable VSB lay er
studio-cli vxbs vsb set  - Set for VSB lay er
studio-cli vxbs vsb add
Add VSB lay er
Synopsis
Add VSB lay er
studio-cli vxbs vsb add [flags]
Options
-b, --branch string   Imported layer git branch
-g, --giturl string   Imported layer git url
-h, --help            help for add
-p, --path string     Imported layer git repo path
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VSB Project id to do config
-q, --jq string         jq query string
-n, --name string       VSB Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vsb  - Conﬁg VSB

<!-- Page 668 -->

studio-cli vxbs vsb create
Vsb create
Synopsis
Vsb create
studio-cli vxbs vsb create [flags]
Options
-a, --access string               sharing access level, LEAD|EDITOR|TESTER|VIEWER
(default "LEAD")
-f, --access-config-name string   Access config name
-b, --base string                 VSB base on
--board string                VSB board
--bspurl string               VSB based on custom BSP
--cpu string                  VSB cpu
--debug                       VSB debug mode
--debug-opt                   VSB debug mode
-d, --desc string                 Optional description for project
--endian string               VSB endian
--fp string                   VSB float
-p, --gitlab-group-name string    Gitlab group name
-g, --groupname string            group name
-h, --help                        help for create
--ilp32                       VSB data model ilp32
--inet4                       VSB select inet4
--inet6                       VSB select inet6
--linker string               VSB linker
--lp64                        VSB data model lp64
-n, --name string                 Name for project
--primaryonly                 VSB compiler only enable primary compiler, option is
only available prior to SR0600
--profile string              VSB profile (Maximum|Minimal|Hardened|DEVELOPMENT)
--psl string                  VSB Platform specific layer
-r, --release string              Release id
--releaseurl string           Release url
--smp                         VSB processor smp mode
--tool string                 VSB tool
-t, --type string                 'private' or 'public' or 'shared' role type, default to
'public' (default "public")
--up                          VSB processor up mode
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VSB Project id to do config
-q, --jq string         jq query string

<!-- Page 669 -->

--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vsb  - Conﬁg VSB
studio-cli vxbs vsb disable
Disable VSB lay er
Synopsis
Disable VSB lay er
studio-cli vxbs vsb disable [flags]
Options
-h, --help            help for disable
-o, --option string   layer or vsb name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VSB Project id to do config
-q, --jq string         jq query string
-n, --name string       VSB Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vsb  - Conﬁg VSB
studio-cli vxbs vsb enable
Enable VSB lay er

<!-- Page 670 -->

Synopsis
Enable VSB lay er
studio-cli vxbs vsb enable [flags]
Options
-h, --help             help for enable
-o, --option strings   layer or vsb name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VSB Project id to do config
-q, --jq string         jq query string
-n, --name string       VSB Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vsb  - Conﬁg VSB
studio-cli vxbs vsb set
Set for VSB lay er
Synopsis
Set for VSB lay er
studio-cli vxbs vsb set [flags]
Options
-h, --help            help for set
-o, --option string   layer or vsb name
-v, --value string    VSB option value

<!-- Page 671 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-i, --id string         VSB Project id to do config
-q, --jq string         jq query string
-n, --name string       VSB Project name to do config
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli vxbs vsb  - Conﬁg VSB
studio-cli ws
Workspace Management commands
Synopsis
Workspace Management commands.
Options
-h, --help        help for ws
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 672 -->

## See Also
studio-cli  - Wind Riv er Studio Command Line Interface tool
studio-cli ws instance  - Manage workspace instance
studio-cli ws namespace  - Manage workspace namespace
studio-cli ws template  - Manage workspace templates
studio-cli ws instance
Manage workspace instance
Synopsis
Manage workspace instance
Options
-h, --help   help for instance
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 673 -->

## See Also
studio-cli ws  - Workspace Management commands
studio-cli ws instance create  - Create a workspace instance
studio-cli ws instance delete  - Delete a workspace instance by (name, templatename, username) or id
studio-cli ws instance extend  - Extend the session of a workspace instance by (name, templatename,
username) or id
studio-cli ws instance list  - Retriev e a list of workspace instances
studio-cli ws instance restart  - Restart a workspace instance by (name, templatename, username) or
id
studio-cli ws instance ssh  - Run the ssh wrapper for a workspace and or dynamically add your ssh
key
studio-cli ws instance start  - Start a workspace instance by (name, templatename, username) or id
studio-cli ws instance status  - Get status of a workspace instance by id
studio-cli ws instance stop  - Stop a workspace instance by (name, templatename, username) or id
studio-cli ws instance create
Create a workspace instance
Synopsis
This command creates a workspace instance based on a speciﬁed template. it will create and start the
instance, which may take some time to complete.
studio-cli ws instance create [flags]
Options
-f, --file string                Override the workspace template definition YAML file
with a custom configuration
-h, --help                       help for create
-n, --name string                The name of the workspace instance to create. max 30
chars and can only contains letters, numbers, underscore and hyphen characters
-r, --ram-resource-name string   The name of the RAM resource for user secret access
management (group access)
-d, --runtime-duration string    Number of hours to run the workspace instance
-s, --secret-path string         The path of user secret in vault (user access)
-t, --template string            The RBAC name of the workspace template
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests

<!-- Page 674 -->

--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws instance  - Manage workspace instance
studio-cli ws instance delete
Delete a workspace instance by (name, templatename, username) or id
Synopsis
Delete a workspace instance by its unique id, or the tuple of created-by, name, and template-name
studio-cli ws instance delete [flags]
Options
-c, --created-by string      Workspace instance creator
-h, --help                   help for delete
-i, --id string              Workspace instance unique ID
-n, --name string            Workspace instance name
-t, --template-name string   Workspace template name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws instance  - Manage workspace instance

<!-- Page 675 -->

studio-cli ws instance extend
Extend the session of a workspace instance by (name, templatename, username) or id
Synopsis
Extend the session of a workspace instance by its unique id, or the tuple of created-by, name, and
template-name
studio-cli ws instance extend [flags]
Options
-c, --created-by string      Workspace instance creator name
-h, --help                   help for extend
-i, --id string              Workspace instance unique ID
-n, --name string            Workspace instance name to extend
-r, --runtime-duration int   Number of hours to extend the workspace instance
-t, --template-name string   Workspace template name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws instance  - Manage workspace instance
studio-cli ws instance list
Retriev e a list of workspace instances
Synopsis
Retriev e a paginated, sorted, ﬁltered list of workspace instances
studio-cli ws instance list [flags]

<!-- Page 676 -->

Options
-c, --created-by string              Creator of the instance
-f, --field string                   The field to sort by (possible values: uptime, name,
status, remainingTime) (default "uptime")
-h, --help                           help for list
-i, --id string                      Instance unique identifier
-l, --limit int                      Page size (default 50)
-n, --name string                    Name of the workspace instance
-o, --order-by string                Sort in ascending or descending order (possible
values: ASC, DESC) (default "ASC")
-e, --ostype string                  OS of the instance (possible values: linux, windows)
-p, --pagination int                 Page number (default 1)
-s, --status string                  Status of the instance (possible values: CREATED,
STARTING, RUNNING, STOPPED, FAILED)
-m, --template-display-name string   The template display name the instance was created
from
-t, --template-name string           The RBAC name of the workspace template the instance
was created from
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws instance  - Manage workspace instance
studio-cli ws instance restart
Restart a workspace instance by (name, templatename, username) or id
Synopsis
Restart a workspace instance by its unique id, or the tuple of created-by, name, and template-name
studio-cli ws instance restart [flags]

<!-- Page 677 -->

Options
-c, --created-by string      Workspace instance creator
-h, --help                   help for restart
-i, --id string              Workspace instance unique ID
-n, --name string            Workspace instance name
-t, --template-name string   Workspace template name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws instance  - Manage workspace instance
studio-cli ws instance ssh
Run the ssh wrapper for a workspace and or dynamically add your ssh key
Synopsis
This command can add an ssh key to your existing workspace(s), as w ell as ssh directly into the
workspace or issue an ssh command.
Any arguments passed to the ssh sub command which are not processed by studio-cli will be passed
directly to the ssh command.
If you need to register a user:
studio-cli ws instance ssh --register-user username
studio-cli ws instance ssh -u username
If you need to add an ssh key dynamically you could run:
studio-cli ws instance ssh --addkey path-to-public-key
studio-cli ws instance ssh -a path-to-public-key
You can also use the workspace id to perform an ssh command and the tab completion can be used on
the –id argument:

<!-- Page 678 -->

studio-cli ws instance ssh –id d40b2057-b10b-46f5-9f26-cf14bﬀd7bf6 studio-cli ws instance ssh -i
d40b2057-b10b-46f5-9f26-cf14bﬀd7bf6
or –name with –template-name and –created-by:
studio-cli ws instance ssh –name instance-name –template-name name-of-the-parent-template-of-
instance –created-by creator-of-the-workspace-instance studio-cli ws instance ssh -n instance-name -t
name-of-the-parent-template-of-instance -c creator-of-the-workspace-instance
If you do not w ant to execute the ssh command directly you can use the –show or -s ﬂag which will
return the command that would hav e been executed.
studio-cli ws instance ssh --id d40b2057-b10b-46f5-9f26-cf14bffd7bf6 --show
studio-cli ws instance ssh -i d40b2057-b10b-46f5-9f26-cf14bffd7bf6 -s
studio-cli ws instance ssh [flags]
Options
-a, --addkey string          Add an ssh key dynamically to the ssh portal
-c, --created-by string      Name of the creator of the workspace instance
-g, --generate               Create a dynamic local ssh key
-h, --help                   help for ssh
-i, --id string              Id of workspace instance
-n, --name string            Name of the workspace instance to launch
-u, --register-user string   Register a user into the ssh portal - must be a unique
username
-s, --show                   Print the ssh command and exit
-t, --template-name string   Name of the workspace template of the instance to be
launched
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws instance  - Manage workspace instance
studio-cli ws instance start
Start a workspace instance by (name, templatename, username) or id

<!-- Page 679 -->

Synopsis
Start a workspace instance by its unique id, or the tuple of created-by, name, and template-name
studio-cli ws instance start [flags]
Options
-c, --created-by string         Workspace instance creator
-h, --help                      help for start
-i, --id string                 Workspace instance unique ID
-n, --name string               Workspace instance name
-r, --runtime-duration string   Number of hours to run the workspace instance
-t, --template-name string      Workspace template name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws instance  - Manage workspace instance
studio-cli ws instance status
Get status of a workspace instance by id
Synopsis
Get status of a workspace instance by its unique id, Optionally. specify a delay before the ﬁrst status
check using the –delay ﬂag.
studio-cli ws instance status [flags]
Options
-h, --help              help for status
-i, --id string         Workspace instance unique ID
-d, --retry-after int   Optional delay (in seconds) before the first status check

<!-- Page 680 -->

Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws instance  - Manage workspace instance
studio-cli ws instance stop
Stop a workspace instance by (name, templatename, username) or id
Synopsis
Stop a workspace instance by its unique id, or the tuple of created-by, name, and template-name
studio-cli ws instance stop [flags]
Options
-c, --created-by string      Workspace instance creator
-h, --help                   help for stop
-i, --id string              Workspace instance unique ID
-n, --name string            Workspace instance name
-t, --template-name string   Workspace template name
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 681 -->

## See Also
studio-cli ws instance  - Manage workspace instance
studio-cli ws namespace
Manage workspace namespace
Synopsis
Manage workspace namespace
Options
-h, --help   help for namespace
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws  - Workspace Management commands
studio-cli ws namespace provision  - Provision kubernetes namespace for workspace
studio-cli ws namespace provision
Provision kubernetes namespace for workspace
Synopsis
Provision kubernetes namespace for workspace
studio-cli ws namespace provision [flags]

<!-- Page 682 -->

Options
-h, --help          help for provision
-n, --name string   The name of the provisional namespace. max 30 chars and can only
contain letters, numbers, underscore and hyphen characters
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws namespace  - Manage workspace namespace
studio-cli ws template
Manage workspace templates
Synopsis
Manage workspace templates
Options
-h, --help        help for template
-q, --jq string   jq query string
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 683 -->

## See Also
studio-cli ws  - Workspace Management commands
studio-cli ws template assign  - Assign a group with role to a workspace template
studio-cli ws template copy  - Copy the content of an existing template
studio-cli ws template create  - Create a new workspace template
studio-cli ws template delete  - Delete a workspace template deﬁnition by name
studio-cli ws template list  - Return a list of workspace templates
studio-cli ws template update  - Update an existing workspace template
studio-cli ws template assign
Assign a group with role to a workspace template
Synopsis
Assign a group with role to a workspace template
studio-cli ws template assign [flags]
Options
-g, --group string   The RBAC group name
-h, --help           help for assign
-n, --name string    The RBAC template name of workspace
-r, --role string    The RBAC role to assign to the group
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws template  - Manage workspace templates

<!-- Page 684 -->

studio-cli ws template copy
Copy the content of an existing template
Synopsis
This command creates a new template by copying the content of an existing template
studio-cli ws template copy [flags]
Options
-d, --dest string   Name of the new template to create
-h, --help          help for copy
-s, --src string    Name of the workspace template to copy
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws template  - Manage workspace templates
studio-cli ws template create
Create a new workspace template
Synopsis
Create a new workspace template
studio-cli ws template create [flags]
Options
-f, --file string    YAML formatted template definition file path
-g, --group string   The RBAC group name

<!-- Page 685 -->

-h, --help           help for create
-n, --name string    The RBAC template name of workspace
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws template  - Manage workspace templates
studio-cli ws template delete
Delete a workspace template deﬁnition by name
Synopsis
This command deletes a workspace template based on template name.
studio-cli ws template delete [flags]
Options
-h, --help          help for delete
-n, --name string   Template name to delete
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code

<!-- Page 686 -->

## See Also
studio-cli ws template  - Manage workspace templates
studio-cli ws template list
Return a list of workspace templates
Synopsis
This command returns a list of workspace templates ﬁltered based on supported ﬁlters (e.g.,
displayName).
studio-cli ws template list [flags]
Options
-d, --display-name string   The display name of the workspace template to search for
-h, --help                  help for list
-i, --id string             The id of the workspace template to search for
-n, --name string           The RBAC name of the workspace template to search for
-t, --os-type string        The os type of the workspace template to search for
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws template  - Manage workspace templates
studio-cli ws template update
Update an existing workspace template
Synopsis
Update an existing workspace template

<!-- Page 687 -->

studio-cli ws template update [flags]
Options
-f, --file string   YAML formatted template definition file path
-h, --help          help for update
-n, --name string   The RBAC name of the workspace template to update
Options inherited from parent commands
--debughttp         Print information for http transactions and timings
--debughttp2        Print extended debug data for http requests
--debughttp3        Print extended debug data for http responses
--debughttp4        Pretty print debug data for http responses and requests.
-q, --jq string         jq query string
--non-interactive   Disable all interactive mode
--output            Set Output Format: [json|yaml]
--raw               Strip single result jq queries of quotes
--totpcode string   TOTP code
## See Also
studio-cli ws template  - Manage workspace templates
