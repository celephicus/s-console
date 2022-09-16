#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

#include "t_support.h"

TT_BEGIN_FIXTURE(setupTestInitFixture, NULL, destroyTestInitFixture);

static ScContext* t_ctx;
void setupTestInitFixture() {
	t_ctx = malloc(sizeof(ScContext));
	memset(t_ctx, 0xee, sizeof(ScContext));
	memset(&g_sc_state, 0xee, sizeof(ScState));
}
void destroyTestInitFixture() {
	free(t_ctx);
	t_ctx = NULL;
}

void testInitContext(void) {
    scInit(t_ctx);
    TEST_ASSERT_EQUAL_PTR(t_ctx, g_sc_state.ctx);
    TEST_ASSERT_EQUAL_PTR(CTX, g_sc_state.ctx);
	TEST_ASSERT_EQUAL_SC_CELL(0, g_sc_state.here);
	TEST_ASSERT_EQUAL_SC_CELL(HERE, g_sc_state.here);
	TEST_ASSERT_EQUAL_SC_CELL(0, g_sc_state.latest);
	TEST_ASSERT_EQUAL_SC_CELL(LATEST, g_sc_state.latest);
}