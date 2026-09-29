global_git_repo = 'https://gitgerrit.asux.aptiv.com/CORECOMP/ALSW/CT_CalibrationTool'
main_description = '<hr width=60% align=left><font color="red"><b>This view was automatically generated via JobDSL! Any manual modification here will get lost!</b></font><br><hr width=60% align=left>'

// create job
def job_name = 'Calibration_Tool_Build'
def job_cr = multibranchPipelineJob(job_name)
def job_id = '38130c62-e18b-4c00-892a-71aee03b0aaf'
def jenkinsfile = 'jenkins/jobs/Calibration_Tool_Build.jenkins'

job_cr.with {
    description(main_description)
    displayName(job_name)
    branchSources {
        branchSource {
            buildStrategies {
                buildChangeRequests {
                    ignoreTargetOnlyChanges(false)
                    ignoreUntrustedChanges(false)
                }
                buildNamedBranches {
                    filters {
                        wildcards {
                                includes("*")
                                excludes("*")
                                caseSensitive(false)
                        }
                    }
                }
            }
            source {
                gerrit {
                    id(job_id) // IMPORTANT: use a constant and unique identifier
                    remote(global_git_repo)
                    credentialsId("GSS-GERRIT-gid_ad_oem_integ")
                    traits {
                        gitBranchDiscovery()
                        changeDiscoveryTrait {
                            queryString("")
                        }
                        headWildcardFilter {
                            includes("*")
                            excludes("")
                        }
                    }
                }
            }
        }
    }
    factory {
        workflowBranchProjectFactory {
            scriptPath(jenkinsfile)
        }
    }
    orphanedItemStrategy {
        discardOldItems {
            daysToKeep(2)
        }
    }
}

// create view
def view_cr = listView('20_Calibration_Tool')
view_cr.with {
    description(main_description)
    columns {
        status()
        weather()
        buildButton()
        disableProject(true)
        name()
        lastSuccess()
        lastFailure()
        lastDuration()
        issuesTotalColumn {
            name("# Issues")
            type("TOTAL")
        }
    }
    statusFilter(StatusFilter.ALL)
    jobs {
        name(job_name)
    }
}
