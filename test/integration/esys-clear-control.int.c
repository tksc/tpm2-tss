/* SPDX-License-Identifier: BSD-2-Clause */
/*******************************************************************************
 * Copyright 2017-2018, Fraunhofer SIT sponsored by Infineon Technologies AG
 * All rights reserved.
 *******************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h" // IWYU pragma: keep
#endif

#include <stdlib.h>           // for EXIT_FAILURE, EXIT_SUCCESS

#include "test-esys.h"        // for EXIT_SKIP, goto_error_if_not_failed
#include "tss2_common.h"      // for TSS2_RC
#include "tss2_esys.h"        // for ESYS_TR_NONE, Esys_ClearControl, ESYS_T...
#include "tss2_tpm2_types.h"  // for TPM2_NO, TPM2_RC_BAD_AUTH, TPM2_RC_HIER...

#define LOGMODULE test
#include "util/log.h"         // for goto_if_error, number_rc, LOG_WARNING

/** Test the ESYS function Esys_ClearControl.
 *
 * The clear command will be disabled and with Esys_Clear it will
 * be checked whether clear is disabled.
 *
 * Tested ESYS commands:
 *  - Esys_Clear() (M)
 *  - Esys_ClearControl() (M)
 *
 * *\b Note: platform authorization needed.
 *
 * @param[in,out] esys_context The ESYS_CONTEXT.
 * @retval EXIT_FAILURE
 * @retval EXIT_SUCCESS
 */
int
test_esys_clear_control(ESYS_CONTEXT * esys_context)
{
    TSS2_RC r;
    int failure_return = EXIT_FAILURE;

    ESYS_TR auth_handle = ESYS_TR_RH_PLATFORM;
    TPMI_YES_NO disable = TPM2_YES;

    r = Esys_ClearControl(
        esys_context,
        auth_handle,
        ESYS_TR_PASSWORD,
        ESYS_TR_NONE,
        ESYS_TR_NONE,
        disable);

    if (number_rc(r) == TPM2_RC_BAD_AUTH ||
        number_rc(r) == TPM2_RC_HIERARCHY) {
        /* Platform authorization not possible test will be skipped */
        LOG_WARNING("Platform authorization not possible.");
        failure_return =  EXIT_SKIP;
        goto error;
    }

    goto_if_error(r, "Error: ClearControl", error);

    r = Esys_Clear (
        esys_context,
        auth_handle,
        ESYS_TR_PASSWORD,
        ESYS_TR_NONE,
        ESYS_TR_NONE);
    goto_error_if_not_failed(r, "Error: ClockSet", error);

    disable = TPM2_NO;

    r = Esys_ClearControl(
        esys_context,
        auth_handle,
        ESYS_TR_PASSWORD,
        ESYS_TR_NONE,
        ESYS_TR_NONE,
        disable);

    goto_if_error(r, "Error: ClearControl", error);

    return EXIT_SUCCESS;

 error:
    return failure_return;
}

int
test_invoke_esys(ESYS_CONTEXT * esys_context) {
    return test_esys_clear_control(esys_context);
}
