#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"
#include "t_support.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

TT_BEGIN_FIXTURE(ts_SetupStackTestContext, NULL, ts_DestroyStackTestContext);

void testInitContext(void) {
    //scInitContext(ctx);
    //TEST_ASSERT_EQUAL_PTR(ctx, sc_ctx);
}