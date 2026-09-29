// Include the Jenkins shared library from CORECOMP/ALSW/Jenkins_SharedLibrary
@Library(value='corecomp-alsw-shared-library@master', changelog=false) _
import helper.Workspace
import publisher.CommonEmailSender
import scm.GitClone

def ML_TEAM_MAIL= 'GDSR_Tracker@aptiv.com; b5738cdf.aptiv.com@amer.teams.ms'

def activateBullseyeIfRequested(){
    script{
        if(Boolean.valueOf(params.GENERATE_BULLSEYE_COVERAGE) && ((params.COMPILER).startsWith('msbuild'))){
            bat(script:  '''cov01 -1''',
                label: "Turn on bullseye coverage")
        }
    }
}
def deactivateBullseye(){
    // This job might run on a machine that does not have bullseye installed:
    // Check if cov01 is available, only if 'where cov01' did not return an error call 'cov01 -0' to deactivate
    bat(script:   '''where cov01
                     if %errorlevel%==0 cov01 -0'''
                     ,
        label: "Turn off bullseye coverage")
}

def COV_PATH="C:/cov-analysis-win64-2020.06/bin"

def build_folder = ""
pipeline {
    agent {
        node {
            label 'Core-Algo-Jenkins'
            customWorkspace("E:\\JenkinsACCP\\" + new Workspace().unique_short_root(env.JOB_NAME, 10, "Windows"))
        }
    }
    parameters {
        choice(
            name: 'COMPILER',
            choices: ["msbuild2015", "msbuild2012", "msbuild2013", "msbuild2017", "clang"],
            description: 'The compiler used for the job' )
        choice(
            name: 'COMPILER_CONFIGURATION',
            choices: ["Both", "Debug", "Release"],
            description: 'The configuration to be used for the job' )
        booleanParam(name: 'CLANG_TIDY', defaultValue: false, description: 'Run clang-tidy\r\n(Available only for clang builds)')
        booleanParam(name: 'RUN_DOXYGEN', defaultValue: true, description: 'Generate the doxygen documentation')
        booleanParam(name: 'RUN_QAC', defaultValue: true, description: 'Run QAC static analysis')
        booleanParam(name: 'RUN_COVERITY', defaultValue: true, description: 'Run Coverity static analysis')
        booleanParam(name: 'COVERITY_CHECK_CORE_MISRA_C2012', defaultValue: true,  description: 'Do MISRA C2012 checks for Gdsr core code')
        booleanParam(name: 'RUN_UNIT_TEST', defaultValue: true, description: 'Run unit tests')
        booleanParam(name: 'RUN_CPP_CHECK', defaultValue: true, description: 'Run CPPcheck static analysis\r\n(Only available for msbuild)')
        booleanParam(name: 'GENERATE_OPENCPP_COVERAGE', defaultValue: true, description: 'Generate a unit test coverage report using openCppCoverage. \r\n (Only available for msbuild) \r\n(Only available in Debug)')
        booleanParam(name: 'GENERATE_BULLSEYE_COVERAGE', defaultValue: true, description: 'Generate a unit test coverage report using Bullseye\r\n(Only available for msbuild)\r\n(Only available in Debug)')
        choice(
            name: 'fast_math',
            choices: ["table", "runtime_table", "function"],
            description: 'Shall the fast math functions use a table, runtime table or call their equivalent from math.h?' )
    }
    environment {
        QACPATH = 'C:/Program Files (x86)/PRQA/QAC-8.0-R/'
        QACBIN  = 'C:/Program Files (x86)/PRQA/QAC-8.0-R/bin'
        QACTEMP = "${WORKSPACE}/qac_temp"
        // Prepare environment variables for Bullseye.
        COVFILE="""${WORKSPACE}/bullseye/coverage.cov"""
        COVERR="""${WORKSPACE}/bullseye/bullseye_error.txt"""
        COVBUILDZONE="""${env.JOB_NAME}"""
        //JFROG_REPO         = "corecomp_devops-aptiv-00000000-buildtools-generic-local"
        //JFROG_BASE_PATH    = "${JFROG_REPO}/Sources/Googletest"
    }
    options {
        buildDiscarder(logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '50'))
        timeout(time: 3, unit: 'HOURS')
        checkoutToSubdirectory('OT')
        disableConcurrentBuilds()
    }
    stages {
        stage('Preparation') {
            steps {
                gerritCommentReview()
                bat("set")
                script {
                    def buildVariant = ""
                    if(params.fast_math=="table"){
                        buildVariant = buildVariant + "fast_math:table"
                    }
                    if(params.fast_math=="runtime_table"){
                        buildVariant = buildVariant + "fast_math:runtime_table"
                    }
                    if(params.fast_math=="function"){
                        buildVariant = buildVariant + "fast_math:function"
                    }
                    currentBuild.displayName = "Commit: " + GIT_COMMIT[0..6] + " " + params.COMPILER + " " + buildVariant

                    dir("OT") {
                        commitMessage = bat(script: "git log --no-walk --format=format:%%s ${GIT_COMMIT}",
                                            returnStdout: true,
                                            label: "Reading git commit message").split('\r\n')[2].trim()
                        currentBuild.description = commitMessage
                    }

                    // def FILES_SPEC = [
                    //     "${env.JFROG_BASE_PATH}/googletest-release-1.11.0.zip": "googletest",
                    // ]
                    // artifactoryDownload(FILES_SPEC)
                    // bat("tar xf googletest/googletest-release-1.11.0.zip")
                    getGoogletest()
                }
            }
        }
        stage('CMake') {
            steps {
                script{
                    ml_fast_math="use_table"
                    if(params.fast_math=="runtime_table"){
                        ml_fast_math="use_runtime_table"
                    }
                    if(params.fast_math=="function"){
                        ml_fast_math="use_function"
                    }
                    build_folder = """${WORKSPACE}/build_${params.COMPILER}"""

                    // the folder 'bullseye' contains among others a coverage report. Ensure its clean by deleting the full folder in any case.
                    dir("bullseye") {
                        deleteDir()
                    }
                    // In case bullseye is installed it expects to find a COVFILE even if its deactivated
                    if (fileExists("""${COVFILE}""")) {
                        writeFile file:"""${COVFILE}""", text:''
                    }
                    else
                    {
                        dir ('') {
                            writeFile file:"""${COVFILE}""", text:''
                        }
                    }
                    echo "Turn off bullseye coverage for cmake configure"
                    // Cmake tries to invoke the compiler and fails if bullseye is active during configure.
                    deactivateBullseye()
                    dir ("""build_${params.COMPILER}""") {
                        deleteDir()
                        writeFile file:'dummy', text:''
                    }
                    cmake_generator = "Visual Studio 11 2012 Win64"
                    if(params.COMPILER == "msbuild2013"){
                        cmake_generator="Visual Studio 12 2013 Win64"
                    }
                    if(params.COMPILER == "msbuild2015"){
                        cmake_generator="Visual Studio 14 2015 Win64"
                    }
                    if(params.COMPILER == "msbuild2017"){
                        cmake_generator="Visual Studio 15 2017 Win64"
                    }
                    if(params.COMPILER == "clang"){
                        cmake_generator="Ninja Multi-Config"
                    }
                    cmake_clang_tidy = "0"
                    if(Boolean.valueOf(params.CLANG_TIDY)){
                        cmake_clang_tidy = "1"
                    }
                }
                bat(script:  """
                    cd ${build_folder}\r\n
                    set PATH=%PATH:C:\\Program Files (x86)\\BullseyeCoverage\\bin;=%\r\n
                    set PATH=C:\\Program Files\\ninja;%PATH%\r\n
                    cmake %WORKSPACE%/OT -G "${cmake_generator}" ^
                     -DMLMathLibrary_QAC_generation_active:BOOL="1" ^
                     -DMLMathLibrary_QAC_output_folder:STRING="%WORKSPACE%/qac_out" ^
                     -DMLMathLibrary_offer_legacy_headers:BOOL="0" ^
                     -DMLMathLibrary_CLANG_TIDY:BOOL="${cmake_clang_tidy}" ^
                     -DMLMathLibrary_fast_math_exp_table:STRING="${ml_fast_math}" ^
                     -DMLMathLibrary_fast_math_trig_table:STRING="${ml_fast_math}"
                    """,
                    label: "CMake generate")
            }
            post {
                always {
                    archiveArtifacts allowEmptyArchive: true, artifacts: "build_${params.COMPILER}/CMakeFiles/CMakeError.log"
                    archiveArtifacts allowEmptyArchive: true, artifacts: "build_${params.COMPILER}/CMakeFiles/CMakeOutput.log"
                }
            }
        }
        stage('Build MathLibrary: Debug') {
            steps {
                deactivateBullseye()
                bat(script:  """cmake --build ${build_folder} --target MLMathLibrary --config Debug --parallel 4 1>${params.COMPILER}_MLMathLibrary_debug.log""",
                    label: "CMake build MLMathLibrary in Debug configuration")
            }
            post {
                always {
                    archiveArtifacts allowEmptyArchive: true, artifacts: """${params.COMPILER}_MLMathLibrary_debug.log"""

                    script{
                        if((params.COMPILER).startsWith('msbuild')){
                            recordIssues(
                                enabledForFailure: true,
                                tool: msBuild(
                                    name : "msbuild debug",
                                    id: "msbuildDebug",
                                    pattern: """${params.COMPILER}_MLMathLibrary_debug.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }else{
                            recordIssues(
                                enabledForFailure: true,
                                tool: clang(
                                    name : "clang debug",
                                    id: "clangDebug",
                                    pattern: """${params.COMPILER}_MLMathLibrary_debug.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }
                    }
                }
            }
        }
        stage('Build MathLibrary: Release') {
            steps {
                deactivateBullseye()
                bat(script:  """cmake --build ${build_folder} --target MLMathLibrary --config Release --parallel 4 1>${params.COMPILER}_MLMathLibrary_release.log""",
                label: "CMake build MLMathLibrary in Release configuration")
            }
            post {
                always {
                    archiveArtifacts allowEmptyArchive: true, artifacts: """${params.COMPILER}_MLMathLibrary_release.log"""

                    script{
                        if((params.COMPILER).startsWith('msbuild')){
                            recordIssues(
                                enabledForFailure: true,
                                tool: msBuild(
                                    name : "msbuild release",
                                    id: "msBuildRelease",
                                    pattern: """${params.COMPILER}_MLMathLibrary_release.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }else{
                            recordIssues(
                                enabledForFailure: true,
                                tool: clang(
                                    name : "clang release",
                                    id: "clangRelease",
                                    pattern: """${params.COMPILER}_MLMathLibrary_release.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }
                    }
                }
            }
        }
        stage('Build MathLibrary: Coverage') {
            steps {
                activateBullseyeIfRequested()
                bat(script:  """cmake --build ${build_folder} --target MLMathLibrary --config Coverage --parallel 4 1>${params.COMPILER}_MLMathLibrary_coverage.log""",
                label: "CMake build MLMathLibrary in Coverage configuration")
                deactivateBullseye()
            }
            post {
                always {
                    archiveArtifacts allowEmptyArchive: true, artifacts: """${params.COMPILER}_MLMathLibrary_coverage.log"""

                    script{
                        if((params.COMPILER).startsWith('msbuild')){
                            recordIssues(
                                enabledForFailure: true,
                                tool: msBuild(
                                    name : "msbuild Coverage",
                                    id: "msBuildCoverage",
                                    pattern: """${params.COMPILER}_MLMathLibrary_coverage.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }else{
                            recordIssues(
                                enabledForFailure: true,
                                tool: clang(
                                    name : "clang Coverage",
                                    id: "clangCoverage",
                                    pattern: """${params.COMPILER}_MLMathLibrary_coverage.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }
                    }
                }
            }
        }
        stage('Run Unit Test: Release') {
            when{
                expression{return Boolean.valueOf(params.RUN_UNIT_TEST)}
            }
            steps {
                    deactivateBullseye()
                    bat(script:  """cmake --build ${build_folder} --target MLMathLibraryUnitTests --config Release --parallel 4 1>${params.COMPILER}_MLMathLibraryUnitTests_release.log""",
                        label: "CMake build unit tests in Release configuration")
                    bat(script:  """${build_folder}/development/unit_test/Release/MLMathLibraryUnitTests.exe -- --gtest_output=xml:%WORKSPACE%/${params.COMPILER}gtestresults_release.xml""",
                        label: "Running unit tests")

            }
            post {
                always {
                    archiveArtifacts """${params.COMPILER}gtestresults_release.xml"""
                    archiveArtifacts allowEmptyArchive: true, artifacts: """${params.COMPILER}/${params.COMPILER}_MLMathLibraryUnitTests_release.log"""
                    script{
                        if((params.COMPILER).startsWith('msbuild')){
                            recordIssues(
                                enabledForFailure: true,
                                tool: msBuild(
                                    name : "msBuild unit tests release",
                                    id: "msBuildUTRelease",
                                    pattern: """${params.COMPILER}_MLMathLibraryUnitTests_release.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }else{
                            recordIssues(
                                enabledForFailure: true,
                                tool: clang(
                                    name : "clang unit tests release",
                                    id: "clangUTRelease",
                                    pattern: """${params.COMPILER}_MLMathLibraryUnitTests_release.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }
                    }
                }
            }
        }
        stage('Run Unit Test: Debug') {
            when{
                expression{return Boolean.valueOf(params.RUN_UNIT_TEST)}
            }
            steps {
                bat(script:  """cmake --build ${build_folder} --target MLMathLibraryUnitTests --config Debug --parallel 4 1>${params.COMPILER}_MLMathLibraryUnitTests_debug.log""",
                label: "CMake build unit tests in Debug configuration")
                catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                    bat(script:  """${build_folder}/development/unit_test/Debug/MLMathLibraryUnitTests.exe ^
                                    --gtest_output=xml:%WORKSPACE%/${params.COMPILER}gtestresults_debug.xml""",
                        label: "Running unit tests")
                }
            }
            post {
                always {
                    archiveArtifacts """${params.COMPILER}gtestresults_debug.xml"""

                    archiveArtifacts allowEmptyArchive: true, artifacts: """${params.COMPILER}_MLMathLibraryUnitTests_debug.log"""
                    script{
                        if((params.COMPILER).startsWith('msbuild')){
                            recordIssues(
                                enabledForFailure: true,
                                tool: msBuild(
                                    name : "msBuild unit tests debug",
                                    id: "msBuildUTDebug",
                                    pattern: """${params.COMPILER}_MLMathLibraryUnitTests_debug.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }else{
                            recordIssues(
                                enabledForFailure: true,
                                tool: clang(
                                    name : "clang unit tests debug",
                                    id: "clangUTDebug",
                                    pattern: """${params.COMPILER}_MLMathLibraryUnitTests_debug.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }
                    }
                    junit allowEmptyResults: true, testResults: """${params.COMPILER}gtestresults_debug.xml"""
                }
            }
        }
        stage('Run Unit Test: Coverage') {
            when{
                expression{return Boolean.valueOf(params.RUN_UNIT_TEST)}
            }
            steps {
                    activateBullseyeIfRequested()
                    bat(script:  """cmake --build ${build_folder} --target MLMathLibraryUnitTests --config Coverage --parallel 4 1>${params.COMPILER}_MLMathLibraryUnitTests_coverage.log""",
                        label: "CMake build unit tests in Coverage configuration")
                    deactivateBullseye()
                    script{
                        // opencppcoverage only supports msbuild
                        if(Boolean.valueOf(params.GENERATE_OPENCPP_COVERAGE) && ((params.COMPILER).startsWith('msbuild'))){
                            catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                                bat(script:  """"C:/Program Files/OpenCppCoverage/opencppcoverage.exe" --excluded_line_regex "[[:space:]]*else[[:space:]]*"  --excluded_line_regex ".*assert.*" --sources %WORKSPACE%\\OT\\ml_core --export_type cobertura:%WORKSPACE%\\coverage_report.xml ${build_folder}\\development\\unit_test\\Coverage\\MLMathLibraryUnitTests.exe -- --gtest_output=xml:%WORKSPACE%/${params.COMPILER}gtestresults_coverage.xml""",
                                    label: "Running unit tests")
                            }
                        }
                        else{
                            catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                                bat(script:  """${build_folder}/development/unit_test/Coverage/MLMathLibraryUnitTests.exe ^
                                                --gtest_output=xml:%WORKSPACE%/${params.COMPILER}gtestresults_coverage.xml""",
                                    label: "Running unit tests")
                            }
                        }
                    }

            }
            post {
                always {
                    archiveArtifacts """${params.COMPILER}gtestresults_coverage.xml"""
                    script{
                        if(Boolean.valueOf(params.GENERATE_OPENCPP_COVERAGE) && ((params.COMPILER).startsWith('msbuild'))){
                            archiveArtifacts 'coverage_report.xml'
                        }
                        if(Boolean.valueOf(params.GENERATE_BULLSEYE_COVERAGE) && ((params.COMPILER).startsWith('msbuild'))){
                            bat(script:  """covselect --import ${build_folder}/development/unit_test/bullseye.txt -f%COVFILE%""",
                                label: "Filtering Bullseye coverage report")
                            bat(script:  """covhtml -f%COVFILE% --decision %WORKSPACE%/bullseye/coverageHtml""",
                                label: "Create full Bullseye html report")
                            bat(script:  """covsrc -f%COVFILE% -p --decision --html > %WORKSPACE%/bullseye/coverageHtml/file_summary.html""",
                                label: "Create a Bullseye summary of all functions")
                            publishHTML (
                            target: [
                                allowMissing: true,
                                alwaysLinkToLastBuild: true, // If this control and "Keep past HTML reports" are checked, publish the link on project level even if build failed.
                                keepAll: false, // If checked, archive reports for all successful builds, otherwise only the most recent
                                reportDir: "bullseye/coverageHtml",
                                reportFiles: 'index.html,file_summary.html',
                                reportName: "Bullseye coverage report"
                            ])
                            zip zipFile: 'bullseye/bullseye.zip', archive: true, dir: 'bullseye/coverageHtml'
                        }
                    }
                    archiveArtifacts allowEmptyArchive: true, artifacts: """${params.COMPILER}_MLMathLibraryUnitTests_coverage.log"""
                    script{
                        if((params.COMPILER).startsWith('msbuild')){
                            recordIssues(
                                enabledForFailure: true,
                                tool: msBuild(
                                    name : "msBuild unit tests Coverage",
                                    id: "msBuildUTCoverage",
                                    pattern: """${params.COMPILER}_MLMathLibraryUnitTests_coverage.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }else{
                            recordIssues(
                                enabledForFailure: true,
                                tool: clang(
                                    name : "clang unit tests Coverage",
                                    id: "clangUTCoverage",
                                    pattern: """${params.COMPILER}_MLMathLibraryUnitTests_coverage.log"""
                                ),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }
                    }
                    junit allowEmptyResults: true, testResults: """${params.COMPILER}gtestresults_coverage.xml"""
                    script{
                        // opencppcoverage only supports msbuild
                        if(Boolean.valueOf(params.GENERATE_OPENCPP_COVERAGE) && ((params.COMPILER).startsWith('msbuild'))){
                            cobertura autoUpdateHealth: false, autoUpdateStability: false, coberturaReportFile: 'coverage_report.xml', conditionalCoverageTargets: '70, 0, 0', failUnhealthy: false, failUnstable: false, lineCoverageTargets: '80, 0, 0', maxNumberOfBuilds: 0, methodCoverageTargets: '80, 0, 0', onlyStable: false, sourceEncoding: 'ASCII', zoomCoverageChart: false
                        }
                    }
                }
            }
        }
        stage('Static Code Analysis') {
            parallel {
                stage(' Code Analysis: CPPCheck') {
                    when{
                        expression{return (Boolean.valueOf(params.RUN_CPP_CHECK)  && ((params.COMPILER).startsWith('msbuild')))}
                    }
                    steps{
                        bat(script:  """"C:/Program Files/Cppcheck/cppcheck.exe" --project=${build_folder}/ml_core/MLMathLibrary.vcxproj --enable=warning,performance,information,style --inconclusive  --inline-suppr --xml --xml-version=2 2> ${WORKSPACE}/cppcheck.xml""",
                            label: "Running CppCheck")
                    }
                    post {
                        always {
                            recordIssues(
                                enabledForFailure: true,
                                tool: cppCheck(pattern: 'cppcheck.xml'),
                                qualityGates: [[threshold: 2, type: 'TOTAL']]
                            )
                            archiveArtifacts 'cppcheck.xml'
                        }
                    }
                }
                stage('Code Analysis: QAC'){
                    when{
                        expression{return Boolean.valueOf(params.RUN_QAC)}
                    }
                    steps{
                        dir ('qac_temp') {
                            writeFile file:'dummy', text:''
                        }
                        bat(script:  """cmake --build ${build_folder} --target MLMathLibrary""",
                            label: "CMake build MathLibrary in Debug configuration")
                        bat(script:  """"C:/Program Files (x86)/PRQA/QAC-8.0-R/m2cm/bin/qaw.exe" qac %WORKSPACE%/build_${params.COMPILER}/development/static_analysis/qac/QAC/MLMathLibrary_QAC80-R_VC8.prj -etxt""",
                            label: "Running QAC")
                    }
                    post {
                        always {
                            recordIssues(
                                enabledForFailure: true,
                                tool: qacSourceCodeAnalyser(pattern: 'qac_out/*.txt'),
                                qualityGates: [[threshold: 1, type: 'TOTAL']]
                            )
                        }
                    }
                }
                stage('Code Analysis: Coverity'){
                    when{
                        expression{return Boolean.valueOf(params.RUN_COVERITY)}
                    }
                    steps{
                        script{
                            deactivateBullseye()
                            dir ("""build_coverity""") {
                                deleteDir()
                                writeFile file:'dummy', text:''
                            }
                            bat(script:  """
                                cd ${WORKSPACE}/build_coverity\r\n
                                set PATH=%PATH:C:\\Program Files (x86)\\BullseyeCoverage\\bin;=%\r\n
                                set PATH=C:\\Program Files\\ninja;%PATH%\r\n
                                cmake %WORKSPACE%/OT -G "Ninja" ^
                                 -DCMAKE_BUILD_TYPE=Release ^
                                 -DOPEN_CPP_COVERAGE_EXECUTABLE:PATH="C:/Program Files/OpenCppCoverage/OpenCppCoverage.exe" ^
                                 -DMLMathLibrary_DEVELOPER_MODE:BOOL="1" ^
                                 -DMLMathLibrary_MSVS_WARNING_AS_ERROR:BOOL="0" ^
                                 -DMLMathLibrary_UNIT_TESTS:BOOL="1" ^
                                 -DMLMathLibrary_QAC_generation_active:BOOL="1" ^
                                 -DMLMathLibrary_QAC_output_folder:STRING="%WORKSPACE%/qac_out" ^
                                 -DAS_Unit_Test_AS_NON_CLEAN_TYPES:BOOL="0" ^
                                 -DMLMathLibrary_CLANG_TIDY:BOOL="${cmake_clang_tidy}" ^
                                 -DMLMathLibrary_offer_legacy_headers:BOOL="0" ^
                                 -DMLMathLibrary_fast_math_exp_table:STRING="${ml_fast_math}" ^
                                 -DMLMathLibrary_fast_math_trig_table:STRING="${ml_fast_math}" ^
                                 -DCMAKE_CXX_COMPILER:PATH="C:/MinGW/bin/g++.exe" ^
                                 -DCMAKE_C_COMPILER:PATH="C:/MinGW/bin/gcc.exe"
                                """,
                                label: "CMake generate")
                            dir ("""cov-idir""") {
                                deleteDir()
                                // Recreate the directory by creating a text file with an arbitrary name
                                writeFile file:'dummy', text:''
                            }
                            bat(script:  """cd ${WORKSPACE}/build_coverity
                                            cmake --build . --target clean
                                            ${COV_PATH}/cov-build --dir %WORKSPACE%/cov-idir --return-emit-failures --emit-complementary-info ninja MLMathLibrary""",
                                label: "Building")

                            def misra_c = ""
                            if(Boolean.valueOf(params.COVERITY_CHECK_CORE_MISRA_C2012)){
                                misra_c = """--coding-standard-config %WORKSPACE%/OT/coverity/Aptiv_IDI_MISRA_C2012_cs2.config"""
                            }
                            analyze_result = bat(script:  """cd %WORKSPACE%/build_coverity
                                            ${COV_PATH}/cov-analyze --dir %WORKSPACE%/cov-idir  --security --concurrency --enable-fnptr --enable-constraint-fpp --enable-virtual --checker-option DEADCODE:no_dead_default:true --checker-option RESOURCE_LEAK:allow_main:true --strip-path %WORKSPACE%/OT  """ + misra_c,
                                returnStdout: true,
                                label: "Analyze")
                            commit_result = bat(script:  """cd %WORKSPACE%/build_coverity
                                        ${COV_PATH}/cov-commit-defects --dir %WORKSPACE%/cov-idir --host coverity.asux.aptiv.com --stream 10027594_09_SharedToolbox --auth-key-file C:/Users/kzpyjy/auth/committer_dewup02.auth --https-port 443 --description "SharedToolbox change set ${GIT_COMMIT}" """,
                                returnStdout: true,
                                label: "Commit")
                            currentBuild.description = "Coverity Analyze\n================\n" + analyze_result +"\n\n\nCoverity Commit\n===============\n" + commit_result
                        }
                    }
                }
            }
        }
        stage('Build Doxygen') {
            when{
                expression{return Boolean.valueOf(params.RUN_DOXYGEN)}
            }
            steps {
                dir("OT/ml_core") {
                    bat(script:  """doxygen 2>${params.COMPILER}_doxygen.log""",
                        label: "Running doxygen")
                }
            }
            post {
                always {
                    recordIssues(
                        enabledForFailure: true,
                        tool: doxygen(pattern: """OT/ml_core/${params.COMPILER}_doxygen.log""")
                    )
                    archiveArtifacts allowEmptyArchive: true, artifacts: """OT/ml_core/${params.COMPILER}_doxygen.log"""
                    publishHTML (target: [
                      allowMissing: false,
                      alwaysLinkToLastBuild: false,
                      keepAll: true,
                      reportDir: "OT/ml_core/doc/html",
                      reportFiles: 'index.html',
                      reportName: "Doxygen MathLibrary"
                    ])
                }
            }
        }
    }
    post {
        always {
            bat 'set > env.txt'
            archiveArtifacts 'env.txt'
        }
        success {
            echo "Build successful!"
            script {
                def email_config = [
                    "attach_log": true
                ]
                def emailer = new CommonEmailSender(this, env, email_config=email_config)
                emailer.print_culprits()
                // emailer.send_success()

                // Set the "Verified" label in Gerrit if it was a build of a changeset.
                if (env.GERRIT_REFNAME != null && env.GERRIT_REFNAME.startsWith("refs/changes/")) {
                    gerritReview labels: [Verified: 1], message: "Verified via ${env.JOB_URL}"
                }
            }
        }
        failure {
            echo "Build failed!"
            script {
                def email_config = [
                    "attach_log": true,
                    "email_recipients": ML_TEAM_MAIL
                ]
                def emailer = new CommonEmailSender(this, env, email_config=email_config)
                emailer.print_culprits()
                emailer.send_fail()

                // Set the "Verified" label in Gerrit if it was a build of a changeset.
                if (env.GERRIT_REFNAME != null && env.GERRIT_REFNAME.startsWith("refs/changes/")) {
                    gerritReview labels: [Verified: -1], message: "Rejected via ${env.JOB_URL}"
                }
            }
        }
        fixed {
            echo "Build fixed!"
            script {
                def email_config = [
                    "email_recipients": ML_TEAM_MAIL
                ]
                def emailer = new CommonEmailSender(this, env, email_config=email_config)
                emailer.print_culprits()
                emailer.send_fixed()
            }
        }
        unstable {
            script {
                // Set the "Verified" label in Gerrit if it was a build of a changeset.
                if (env.GERRIT_REFNAME != null && env.GERRIT_REFNAME.startsWith("refs/changes/")) {
                    gerritReview labels: [Verified: 0], message: "Build is unstable: ${env.JOB_URL}"
                }
            }
        }
        cleanup {
            echo "Cleaning workspace..."
            deleteDir()
        }
    }
}
