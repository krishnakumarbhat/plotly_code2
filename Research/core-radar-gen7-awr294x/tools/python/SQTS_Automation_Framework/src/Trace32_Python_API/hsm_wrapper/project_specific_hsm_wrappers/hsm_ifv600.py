"""
This module contains the tools to handle the HSM script specific to the IFV600 project.
"""

from typing import Literal
import time
import logging

from ..hsm import HSMWrapper

logger = logging.getLogger(__name__)


class IFV600_HSM(HSMWrapper):
    """
    Class to handle the HSM script specific to the IFV600 project.
    Inherits from the general HSMWrapper class.
    """
    def prog_hsm_bm(self, variant: Literal['ADCAM', 'mPAD']) -> None:
        """
        Method to program HSM + BM. Equivalent to pressing the 'ADCAM' or 'mPAD' buttons
        in the 'PROG (HSM + BM)' section of the dialog window.

        Args:
            variant: The variant for which HSM + BM should be programmed.
        """
        logger.debug(f'Programming hsm and bm for hw variant {variant}...')
        match variant.upper():
            case 'ADCAM':
                self.select_cmm_option('prog_hsm_adcam')

            case 'MPAD':
                self.select_cmm_option('prog_hsm_mpad')

            case _:
                logger.error(f'Unrecognized variant: {variant}')
                raise Exception(f'Unrecognized variant: {variant}')

        self.t32.print('SQTS - prog_hsm_bm finished.')

    def prog_cert_store(
            self,
            variant: Literal['ADCAM', 'mPAD'],
            certificate_type: Literal['TEST', 'ROW']
            ) -> None:
        """
        Method to program CertStore. Equivalent to pressing the 'ADCAM' or 'mPAD' buttons
        in the 'PROG CertStore' section of the dialog window after selecting the 'ROW' or 'TEST'
        checkbox.

        Args:
            variant: The variant for which CertStore should be programmed.
            certificate_type: The certificate type to use for programming CertStore.
        """
        logger.debug(f'Programming cert store for hw variant {variant} with certificate type {certificate_type}...')
        certificate_type_mapping = {
            'TEST': 'GHS',
            'ROW': 'DEV'
        }

        if certificate_type not in ['TEST', 'ROW']:
            logger.error(f'Unrecognized certificate type: {certificate_type}')
            raise Exception(f'Unrecognized certificate type: {certificate_type}')

        cert_type = certificate_type_mapping[certificate_type]

        if variant not in ['ADCAM', 'mPAD']:
            logger.error(f'Unrecognized Hardware variant: {variant}')
            raise Exception(f'Unrecognized Hardware variant: {variant}')

        if self._hsm_enabled:
            logger.error('Tried to program cert store with hsm enabled.')
            raise Exception('HSM is enabled, will not flash.')

        if not self.is_open:
            self.run_cmm()

        self.t32.run_cmm(fr'"{self._dir}\TC39x\TC39x.cmm"', 'PREPAREONLY', 'DECLAREHSM')
        self.t32.cmd('GOSUB VERIFY_HSM_DISABLED')
        time.sleep(3)

        hsm_disabled = self.t32.practice.get_macro('&hsmDisabled')

        if not hsm_disabled:
            logger.error('HSM was not disabled, aborting cert_store programming...')
            raise Exception('HSM was not disabled, aborting cert_store programming...')

        self.t32.cmd(fr'&certStorePath="{self._dir}\TC39x\Hsm\Certstore\CertStore_{variant}_{cert_type}.hex"')
        self.t32.cmd('GOSUB PROGRAM_CERTSTORE')
        time.sleep(3)

        self.t32.run_cmm(fr'"{self._dir}\TC39x\t32windows\TC39x_win_core0.cmm"')
        self.t32.cmd('CONTINUE')

        self.wait_for_cmm_to_finish()
        self.t32.print('SQTS - prog_cert_store finished.')


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 05/31/2023  ABPA       FKU-921   Initial creation
