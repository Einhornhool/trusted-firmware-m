/*
 * Copyright (c) 2022-2024 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file    power_control.h
 * \brief   Example implementation for Corstone-3xx Power Control.
 * \details Example implementation of Corstone-3xx Power Control (PI_LEVEL=1).
 *          The following functions help to configure a minimum power state for
 *          each power domain/bounded region, preventing transition into a lower
 *          power state, when the Power Policy Units are configured to dynamic
 *          OFF policy (default configuration).
 *          Note: Direct transtion from one power state to another is not
 *          always possible, please refer to Corstone-3xx Technical Reference
 *          Manual.
 */

#ifndef __POWER_CONTROL_H__
#define __POWER_CONTROL_H__

#include "platform_regs.h"
#include "platform_base_address.h"
#include "core_cm85.h"

/* PDCM_PD_SENSE: MIN_PWR_STATE Position */
#define PDCM_PD_SENSE_MIN_PWR_STATE_Pos     30
/* PDCM_PD_SENSE: MIN_PWR_STATE Mask */
#define PDCM_PD_SENSE_MIN_PWR_STATE_Msk    \
    (3UL << PDCM_PD_SENSE_MIN_PWR_STATE_Pos)

/* CPUPWRCFG: TCM_MIN_PWR_STATE Position */
#define CPUPWRCFG_TCM_MIN_PWR_STATE_Pos     4
/* CPUPWRCFG: TCM_MIN_PWR_STATE Mask */
#define CPUPWRCFG_TCM_MIN_PWR_STATE_Msk    \
    (1UL << CPUPWRCFG_TCM_MIN_PWR_STATE_Pos)

static struct mps4_corstone3xx_sysctrl_t *sysctrl =
    (struct mps4_corstone3xx_sysctrl_t*) MPS4_CORSTONE3XX_SYSCTRL_BASE_S;
static struct cpu0_pwrctrl_t *pwrctrl =
    (struct cpu0_pwrctrl_t*) CPU0_PWRCTRL_BASE_S;

/* PDCM minimum power states */
enum PDCM_MIN_PWR_STATES {
    PDCM_MIN_PWR_STATE_OFF = 0,
    PDCM_MIN_PWR_STATE_RET,
    PDCM_MIN_PWR_STATE_ON
};

static inline void
    PD_VMR0_SET_MIN_PWR_STATE(enum PDCM_MIN_PWR_STATES min_pwr_state)
{
    sysctrl->pdcm_pd_vmr0_sense =
        ((sysctrl->pdcm_pd_vmr0_sense & ~PDCM_PD_SENSE_MIN_PWR_STATE_Msk) |
         ((min_pwr_state << PDCM_PD_SENSE_MIN_PWR_STATE_Pos) &
          PDCM_PD_SENSE_MIN_PWR_STATE_Msk));
}

static inline void
    PD_VMR1_SET_MIN_PWR_STATE(enum PDCM_MIN_PWR_STATES min_pwr_state)
{
    sysctrl->pdcm_pd_vmr1_sense =
        ((sysctrl->pdcm_pd_vmr1_sense & ~PDCM_PD_SENSE_MIN_PWR_STATE_Msk) |
         ((min_pwr_state << PDCM_PD_SENSE_MIN_PWR_STATE_Pos) &
          PDCM_PD_SENSE_MIN_PWR_STATE_Msk));
}

static inline void
    PD_SYS_SET_MIN_PWR_STATE(enum PDCM_MIN_PWR_STATES min_pwr_state)
{
    sysctrl->pdcm_pd_sys_sense =
        ((sysctrl->pdcm_pd_sys_sense & ~PDCM_PD_SENSE_MIN_PWR_STATE_Msk) |
         ((min_pwr_state << PDCM_PD_SENSE_MIN_PWR_STATE_Pos) &
          PDCM_PD_SENSE_MIN_PWR_STATE_Msk));
}

static inline void
    PD_CPU0_TCM_SET_MIN_PWR_STATE(enum PDCM_MIN_PWR_STATES min_pwr_state)
{
    pwrctrl->cpupwrcfg =
        ((pwrctrl->cpupwrcfg & ~CPUPWRCFG_TCM_MIN_PWR_STATE_Msk) |
         ((min_pwr_state << CPUPWRCFG_TCM_MIN_PWR_STATE_Pos) &
          CPUPWRCFG_TCM_MIN_PWR_STATE_Msk));
}

/* CPU minimum power states */
enum CPU_MIN_PWR_STATES {
    CPU_MIN_PWR_STATE_ON = 0,
    CPU_MIN_PWR_STATE_ON_CLK_OFF,
    CPU_MIN_PWR_STATE_RET,
    CPU_MIN_PWR_STATE_OFF
};

static inline void
    PD_CPU0_CORE_SET_MIN_PWR_STATE(enum CPU_MIN_PWR_STATES min_pwr_state)
{
    PWRMODCTL->CPDLPSTATE =
        ((PWRMODCTL->CPDLPSTATE & ~PWRMODCTL_CPDLPSTATE_CLPSTATE_Msk) |
         ((min_pwr_state << PWRMODCTL_CPDLPSTATE_CLPSTATE_Pos) &
          PWRMODCTL_CPDLPSTATE_CLPSTATE_Msk));
}

static inline void
    PD_CPU0_EPU_SET_MIN_PWR_STATE(enum CPU_MIN_PWR_STATES min_pwr_state)
{
    PWRMODCTL->CPDLPSTATE =
        ((PWRMODCTL->CPDLPSTATE & ~PWRMODCTL_CPDLPSTATE_ELPSTATE_Msk) |
         ((min_pwr_state << PWRMODCTL_CPDLPSTATE_ELPSTATE_Pos) &
          PWRMODCTL_CPDLPSTATE_ELPSTATE_Msk));
}

static inline void
    PD_CPU0_RAM_SET_MIN_PWR_STATE(enum CPU_MIN_PWR_STATES min_pwr_state)
{
    PWRMODCTL->CPDLPSTATE =
        ((PWRMODCTL->CPDLPSTATE & ~PWRMODCTL_CPDLPSTATE_RLPSTATE_Msk) |
         ((min_pwr_state << PWRMODCTL_CPDLPSTATE_RLPSTATE_Pos) &
          PWRMODCTL_CPDLPSTATE_RLPSTATE_Msk));
}

static inline void
    PD_CPU0_DEBUG_SET_MIN_PWR_STATE(enum CPU_MIN_PWR_STATES min_pwr_state)
{
    PWRMODCTL->DPDLPSTATE =
        ((PWRMODCTL->DPDLPSTATE & ~PWRMODCTL_DPDLPSTATE_DLPSTATE_Msk) |
         ((min_pwr_state << PWRMODCTL_DPDLPSTATE_DLPSTATE_Pos) &
          PWRMODCTL_DPDLPSTATE_DLPSTATE_Msk));
}

static inline void BR_SYS_SET_MIN_PWR_STATE_OFF(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_MEM_RET_OPMODE0(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_MEM_RET_OPMODE1(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_MEM_RET_OPMODE2(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_MEM_RET_OPMODE3(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_FULL_RET_OPMODE0(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_FULL_RET_OPMODE1(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_FULL_RET_OPMODE2(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_FULL_RET_OPMODE3(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_ON_OPMODE0(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_ON_OPMODE1(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_ON_OPMODE2(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
}

static inline void BR_SYS_SET_MIN_PWR_STATE_ON_OPMODE3(void)
{
    PD_SYS_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
    PD_VMR0_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
    PD_VMR1_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_OFF(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_OFF);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_MEM_RET(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_MEM_RET_NOCACHE(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_LOGIC_RET(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_LOGIC_RET_NOCACHE(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_FULL_RET(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_FULL_RET_NOCACHE(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_RET);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_EPU_OFF(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_EPU_OFF_NOCACHE(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_FUNC_RET(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_FUNC_RET_NOCACHE(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_RET);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_ON(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
}

static inline void BR_CPU0_SET_MIN_PWR_STATE_ON_NOCACHE(void)
{
    PD_CPU0_CORE_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_EPU_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_ON);
    PD_CPU0_RAM_SET_MIN_PWR_STATE(CPU_MIN_PWR_STATE_OFF);
    PD_CPU0_TCM_SET_MIN_PWR_STATE(PDCM_MIN_PWR_STATE_ON);
}

#endif /* __POWER_CONTROL_H__ */
