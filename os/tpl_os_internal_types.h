/**
 * @file tpl_os_internal_types.h
 *
 * @section descr File description
 *
 * Trampoline internal types. These types are not intended
 * to be used in an application.
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 * Copyright ESEO for function and data structures documentation
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#ifndef TPL_OS_INTERNAL_TYPES_H
#define TPL_OS_INTERNAL_TYPES_H

#include "tpl_os_custom_types.h"
#include "tpl_machine.h"
#include "tpl_compiler.h"

/**
 * @def CONFORM_ECC1
 *
 * identifies the ECC1 conformance class
 *
 * @see #CONFORMANCE_CLASS
 */
#define CONFORM_ECC1        1

/**
 * @def CONFORM_ECC2
 *
 * identifies the ECC2 conformance class
 *
 * @see #CONFORMANCE_CLASS
 */
#define CONFORM_ECC2        2

/**
 * @def CONFORMANCE_CLASS
 *
 * represents the configured conformance class
 *
 * @warning at this time, Trampoline only supports ECC2
 */
#define CONFORMANCE_CLASS   CONFORM_ECC2

/**************************************
 * Special result code for Trampoline *
 **************************************/

/**
 * @def NO_SPECIAL_CODE
 *
 * Nothing particular to Trampoline in this code.
 *
 * @note This is a Trampoline result code, not an OSEK one.
 *
 * @see #tpl_status
 */
#define NO_SPECIAL_CODE         0

/**
 * @def NEED_RESCHEDULING
 *
 * Scheduler have to be called, probably because the ready list
 * has changed.
 *
 * @note This is a Trampoline result code, not an OSEK one.
 *
 * @see #tpl_status
 */
#define NEED_RESCHEDULING       32

/**
 * @def E_OK_AND_SCHEDULE
 *
 * Combines OSEK-E_OK and Trampoline-NEED_RESCHEDULING result codes
 *
 * @see #tpl_status
 */
#define E_OK_AND_SCHEDULE   ((tpl_status)E_OK | (tpl_status)NEED_RESCHEDULING)

/**
 * @def OSEK_STATUS_MASK
 *
 * This AND mask can be used to keep only result code
 * specific to OSEK in a tpl_status (StatusType)
 * result code.
 *
 * @see #tpl_status
 */
#define OSEK_STATUS_MASK        0x1F

/**
 * @def TRAMPOLINE_STATUS_MASK
 *
 * This AND mask can be used to keep only result code
 * specific to Trampoline in a tpl_status (StatusType)
 * result code.
 *
 * @see #tpl_status
 */
#define TRAMPOLINE_STATUS_MASK  0xE0

/**
 * @typedef tpl_bool
 *
 * Standard bool declaration. Values can be #TRUE or #FALSE.
 */
typedef u8 tpl_bool;

/**
 * @typedef tpl_status
 *
 * This type is used for return
 * status of services (ie one of the result
 * codes specified below). 
 *
 * It is binary compatible with OSEK StatusType, provided
 * Trampoline specific codes are removed (via #OSEK_STATUS_MASK
 * AND-mask). 
 *
 * @see #OSEK_STATUS_MASK
 * @see #StatusType
 */
typedef u8 tpl_status;

/**
 * @typedef tpl_callback_func
 *
 * This type is used for various
 * callback function type in Trampoline.
 */
/**********************************************************************
**  J.Monsimier  22/05/2007 
**  PR09: for S12: need to add __near for the compiler or the 
**        pointer is considered at far and there is a stack pointer 
**        error during function return in tpl_action_activate_task
**********************************************************************/
typedef void (*OS_CBK_CODE tpl_callback_func)(void);

/************************
 * Forward declarations *
 ************************/
struct TPL_TASK;
struct TPL_RESOURCE;
struct TPL_ACTION;
struct TPL_COUNTER;
struct TPL_TIME_OBJ;


/**
 * @typedef tpl_exec_obj_type 
 * This type is used to distinguish kind
 * of tasks and ISR
 *
 * @see #tpl_is_basic
 * @see #tpl_is_extended
 * @see #tpl_is_full_preemptable
 * @see #tpl_is_non_preemptable
 * @see #tpl_is_isr
 */
typedef u8 tpl_exec_obj_type;
 
/**
 * @typedef tpl_exec_state
 *
 * This type represents the state of a task.
 *
 * See page 16+ of the OSEK/VDX 2.2.2 spec
 *
 * Value can be one of :
 * - #AUTOSTART
 * - #RUNNING
 * - #READY
 * - #SUSPENDED
 * - #WAITING (only for ECC1 or ECC2 conformance classes)
 */
typedef u8 tpl_exec_state;

/**
 * @def SUSPENDED
 *
 * Task is suspended.
 *
 * @see #tpl_exec_state
 */
#define SUSPENDED       0x0

/**
 * @def READY
 *
 * Task is ready (to be run, or elected by scheduler).
 *
 * @see #tpl_exec_state
 */
#define READY           0x1

/**
 * @def RUNNING
 *
 * Task is currently running.
 *
 * @see #tpl_exec_state
 */
#define RUNNING         0x2

#if (CONFORMANCE_CLASS == CONFORM_ECC1) || (CONFORMANCE_CLASS == CONFORM_ECC2)
/**
 * @def WAITING
 *
 * Task is waiting
 *
 * @see #tpl_exec_state
 */
#define WAITING         0x3
#endif

/**
 * @def DYING
 *
 * Task is dying. This is an transient state
 * used to  tell the scheduler the currently
 * running task relinquishes the CPU and its
 * context should not be saved
 *
 * @see #tpl_exec_state
 */
#define DYING           0x7

/**
 * @def RESURRECT
 *
 * Task is resurrecting. This is an transient state
 * used to  tell the scheduler the currently
 * running task relinquishes the CPU,
 * its context should not be saved,
 * it should be put in the ready list like a new task
 *
 * @see #tpl_exec_state
 */
#define RESURRECT       0xB

/**
 * @def AUTOSTART
 *
 * Task is automatically activated at system startup
 *
 * @see #tpl_exec_state
 */
#define AUTOSTART       0x4

/**
 * @def READY_AND_NEW
 *
 * Task is ready and should be initialized when it get the CPU.
 *
 * @see #tpl_exec_state
 */
#define READY_AND_NEW   0x5

/**
 * @typedef tpl_exec_function
 *
 * This type is used to specify the task entry point (function where the task
 * begins)
 *
 * @see #TPL_EXEC_STATIC
 */
typedef void (*tpl_exec_function)(void);

#ifdef WITH_AUTOSAR_TIMING_PROTECTION
/** 
 * @internal
 *
 * @struct TPL_TIMING_PROTECTION
 *
 * This structure gathers all informations about timing protection
 * of an executable object (task or isr). See AUTOSAR OS SWS �7.6.2
 * about timing protection.
 *
 * @see #tpl_timing_protection
 * @see #tpl_exec_static
 */
struct TPL_TIMING_PROTECTION
{
    tpl_time             execution_budget; /**< maximum duration the task
                                               can be active within a 
                                               timeframe or maximum isr
                                               execution time since last
                                               activation */
    tpl_activate_counter count_limit;     /**< maximum number of activations 
                                               of the isr within a
                                               timeframe */
    tpl_time             timeframe;       /**< configured timeframe for this
                                               timing protection */
    tpl_time *resource_lock_time;         /**< array where timing protection
                                               is specified (or not) for
                                               each resource (zero if no
                                               timing protection) */
    tpl_time os_interrupt_lock_time;
    tpl_time all_interrupt_lock_time;
};

/**
 * @internal
 *
 * this is an alias for the type #TPL_TIMING_PROTECTION
 *
 * @see #TPL_TIMING_PROTECTION
 */
typedef struct TPL_TIMING_PROTECTION tpl_timing_protection;
#endif /* defined WITH_AUTOSAR_TIMING_PROTECTION */

/**
 * @struct TPL_EXEC_STATIC
 *
 * This is a data structure used to describe the members of task descriptors
 * or category 2 Interrupt Service Routines. Static means this part of the
 * descriptor can be stored in ROM.
 */
struct TPL_EXEC_STATIC {
    tpl_context                     context;            /**< context(s) of the
                                                             task/isr */
    tpl_stack                       stack;              /**< stack(s) of the
                                                             task/isr */
    const tpl_exec_function         entry;              /**< function that is
                                                             the entry point
                                                             of the task/isr*/
    struct TPL_INTERNAL_RESOURCE    *internal_resource; /**< pointer to an
                                                             internal resource.
                                                             NULL if the task
                                                             does not have an
                                                             internal resource
                                                             */
    const tpl_task_id               id;                 /**< id of task/isr */
    const tpl_priority              base_priority;      /**< base priority of
                                                             the task/isr  */
    const tpl_activate_counter      max_activate_count; /**< max activation
                                                             count of a 
                                                             task/isr*/
    const tpl_exec_obj_type         type;               /**< type of the
                                                             task/isr */
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    const tpl_timing_protection     *timing_protection; /**< timing protection 
                                                             configuration
                                                             (can be NULL
                                                             if no timing
                                                             protection is
                                                             needed) */
#endif /* WITH_AUTOSAR_TIMING_PROTECTION */
};

/**
 * @typedef tpl_exec_static
 *
 * This is an alias for the #TPL_EXEC_STATIC structure
 *
 * @see #TPL_EXEC_STATIC
 */
typedef struct TPL_EXEC_STATIC tpl_exec_static;

/**
 * @struct TPL_EXEC_COMMON
 *
 * This structure gathers the common members of executable objects dynamic
 * descriptors
 */
struct TPL_EXEC_COMMON {
    tpl_exec_static         *static_desc;   /**<  pointer to static 
                                                  descriptor */
    struct TPL_RESOURCE     *resources;     /**<  head of the ressources 
                                                  held */
    tpl_activate_counter    activate_count; /**<  current activate count */
    tpl_priority            priority;       /**<  current priority */
    tpl_exec_state          state;          /**<  state (READY, RUNING, ...)*/
#ifdef WITH_AUTOSAR_TIMING_PROTECTION
    tpl_time                monitor_start_date;        /**< last start date of
                                                            the task's budget
                                                            monitor or date
                                                            of last isr
                                                            activation */
    tpl_time                time_left;                 /**< time left before
                                                            exceeding execution
                                                            budget of the task
                                                            or activation
                                                            count left before
                                                            exceeding max
                                                            activation count
                                                            in case of isr2 */
#endif /* WITH_AUTOSAR_TIMING_PROTECTION */
};

/**
 * @typedef tpl_exec_common
 *
 * This is an alias for the #TPL_EXEC_COMMON structure.
 *
 * @see #TPL_EXEC_COMMON
 */
typedef struct TPL_EXEC_COMMON tpl_exec_common;

/**
 * @struct TPL_TASK
 *
 * This structure glues together a common descriptor
 * and the dynamic members of the task descriptor.
 */
struct TPL_TASK {
    tpl_exec_common         exec_desc;  /**< the common descriptor of the 
                                             task */
    tpl_event_mask          evt_set;    /**< @todo what's this ? */
    tpl_event_mask          evt_wait;   /**< @todo what's this ? */
};

/**
 * @typedef tpl_task
 *
 * This type is an alias for the #TPL_TASK structure.
 *
 * @see #TPL_TASK
 */
typedef struct TPL_TASK tpl_task;

/**
 * @typedef tpl_fifo_state
 *
 * This type gathers a read index and a size for fifo management
 */
typedef struct {
    u8 read;
    u8 size;
} tpl_fifo_state;

/**
 * @typedef tpl_priority_level
 *
 * This type is a structure used to store the information for a priority
 * level (pointer to a fifo and the size of the fifo.
 * It is the element od the ready list table.
 */
typedef struct {
    tpl_exec_common **fifo;
    u8 size;
} tpl_priority_level;

/**
 * @struct TPL_RESOURCE
 *
 * This structure describes all attributes of a resource
 */
struct TPL_RESOURCE {
    const tpl_priority      ceiling_priority;     /**< Ceiling priority as 
                                                       computed at system 
                                                       generation time. */
    tpl_priority            owner_prev_priority;  /**< Priority of the owner
                                                       before accessing to the
                                                       resource. This field is
                                                       used to restore the 
                                                       priority of the task 
                                                       when the resource is
                                                       released */
    struct TPL_EXEC_COMMON  *owner;               /**< Owner of the resource 
                                                       or NULL if the resource
                                                       is not owned */
    struct TPL_RESOURCE     *next_res;            /**< Pointer to the next
                                                       resource used to link
                                                       them together when a
                                                       task get more than one
                                                       resource */
};

/**
 * @typedef tpl_resource
 *
 * This type is an alias for the structure #TPL_RESOURCE.
 *
 * @see #TPL_RESOURCE
 */
typedef struct TPL_RESOURCE tpl_resource;

/**
 * @struct TPL_INTERNAL_RESOURCE
 *
 * This is is the internal resource descriptor structure. It is a simpler
 * structure than tpl_resource since only one internal resource can be taken
 * and there is no need to store the owner
 */
struct TPL_INTERNAL_RESOURCE {
    
    const tpl_priority      ceiling_priority;    /**<  Ceiling priority as
                                                       computed at system
                                                       generation time */
    tpl_priority            owner_prev_priority; /**<  Priority of the owner
                                                       prior to the access to
                                                       the resource. This field
                                                       is used to restore the
                                                       priority of the task
                                                       when the resource is
                                                       released */
    tpl_bool                taken;               /**<  Flag to tell if the
                                                       internal resource is
                                                       taken or not */
};

/**
 * @typedef tpl_internal_resource
 *
 * This is an alias for the structure #TPL_INTERNAL_RESOURCE
 *
 * @see #TPL_INTERNAL_RESOURCE
 */
typedef struct TPL_INTERNAL_RESOURCE tpl_internal_resource;

/**
 * @typedef tpl_time_obj_state
 *
 * Describes an time object state. Value can be one of :
 * - #ALARM_SLEEP
 * - #ALARM_ACTIVE
 * - #ALARM_AUTOSTART (only before startup)
 */
typedef u8 tpl_time_obj_state;

/**
 * @typedef tpl_alarm_kind
 *
 * Sort of alarm. Value can be one of :
 * - #ALARM_CALLBACK
 * - #ALARM_TASK_ACTIVATION
 * - #ALARM_EVENT_SETTING
 */
typedef u8 tpl_alarm_kind;


/**
 * @typedef tpl_action_func
 *
 * Prototype for action functions
 */
/**********************************************************************
**  J.Monsimier  22/05/2007 
**  PR09: for S12: need to add __near for the compiler or the 
**        pointer is considered at far and there is a stack pointer 
**        error during function return in tpl_action_activate_task
**********************************************************************/
typedef tpl_status (*OS_CBK_CODE tpl_action_func)(
    const struct TPL_ACTION *
);

/**
 * @struct TPL_ACTION
 *
 * Action base structure
 *
 * This structure contains the pointer to the action function only.
 * It is the common part for the action descriptor structures and is
 * extended to add the action parameters.
 */
struct TPL_ACTION {
  tpl_action_func action;    /**<  action function pointer   */
};

/**
 * @typedef tpl_action
 *
 * This is an alias for the structure #TPL_ACTION
 *
 * @see #TPL_ACTION
 */
typedef struct TPL_ACTION tpl_action;


struct TPL_COUNTER;

/**
 * @typedef tpl_expire_func
 *
 * Prototype for expire functions
 */
typedef tpl_status (*tpl_expire_func)(
    struct TPL_TIME_OBJ *
);

/**
 * @struct TPL_TIME_OBJ_STATIC
 *
 * This is the static data structure used to store the information about
 * an object that can be put in the queue of a counter with an expiration
 * date. This structure complement the dynamic part.
 *
 * @see #TPL_ALARM
 */
struct TPL_TIME_OBJ_STATIC {
    struct TPL_COUNTER      *counter;   /**< a pointer to the counter the
                                             alarm belongs to               */
    tpl_expire_func         expire;     /**< expiration processing to be
                                             done when the time object
                                             expires                        */
};

/**
 * @typedef tpl_time_obj
 *
 * This is an alias for the structure #TPL_TIME_OBJ
 *
 * @see #TPL_TIME_OBJ
 */
typedef struct TPL_TIME_OBJ_STATIC tpl_time_obj_static;

/**
 * @struct TPL_ALARM_STATIC
 *
 * This is the data structure used to describe the static part of an alarm.
 * It extends the #TPL_TIME_OBJ_STATIC structure by adding an action to be done
 * when the alarm expires.
 *
 * @see #TPL_TIME_OBJ_STATIC
 */
struct TPL_ALARM_STATIC {
    tpl_time_obj_static b_desc;     /**< common part of all objects that
                                         derive from tpl_time_obj.          */
    tpl_action          *action;    /**< action to be done when the alarm
                                         expires                            */
};

/**
 * @typedef tpl_alarm_static
 *
 * This is an alias for the structure #TPL_ALARM_STATIC
 *
 * @see #TPL_ALARM_STATIC
 */
typedef struct TPL_ALARM_STATIC tpl_alarm_static;

/**
 * @struct TPL_TIME_OBJ
 *
 * This is the data structure used to describe the dynamic part of,
 * for instance, an alarm or, in future extensions, any dated object
 * that is stored in the queue of a counter.
 */
struct TPL_TIME_OBJ {
    tpl_time_obj_static *stat_part; /**< pointer to the static descriptor   */
    struct TPL_TIME_OBJ *next_to;   /**< next alarm in the active
                                         alarm list                         */
    struct TPL_TIME_OBJ *prev_to;   /**< previous alarm in the active
                                         alarm list                         */
    tpl_tick            cycle;      /**< cycle delay for cyclic alarms      */
    tpl_tick            date;       /**< absolute date of the alarm         */
    tpl_time_obj_state  state;      /**< state of the alarm. An alarm may
                                         have 2 states: ALARM_SLEEP and
                                         ALARM_ACTIVE.
                                         @see #tpl_alarm_state              */
};

/**
 * @typedef tpl_time_obj
 *
 * This is an alias for the structure #TPL_TIME_OBJ
 *
 * @see #TPL_TIME_OBJ
 */
typedef struct TPL_TIME_OBJ tpl_time_obj;

/**
 * @struct TPL_COUNTER
 *
 * This is the data structure used to describe a counter
 */
struct TPL_COUNTER {
    const tpl_tick  ticks_per_base;     /**< number of ticks until the
                                             counter increments             */
    const tpl_tick  max_allowed_value;  /**< maximum allowed value for
                                             a counter                      */
    const tpl_tick  min_cycle;          /**< number of ticks until the
                                             counter increments             */
    tpl_tick        current_tick;   /**< current tick value of the counter  */
    tpl_tick        current_date;   /**< current value of the counter       */
    tpl_time_obj    *first_to;      /**< active alarms list head            */
    tpl_time_obj    *next_to;       /**< next active alarms                 */
};

/**
 * @typedef tpl_counter
 *
 * This is an alias for the structure #TPL_COUNTER
 *
 * @see #TPL_COUNTER
 */
typedef struct TPL_COUNTER tpl_counter;

/*******************************************
 * OSEK/VDX API definitions and prototypes *
 *******************************************/

/**
 * @struct ALARM_BASE_TYPE
 *
 * This structure is defined as paragraph 13.6.1 of OSEK/VDX spec
 *
 * @see AlarmBaseType
 */
struct ALARM_BASE_TYPE {
    tpl_tick  maxallowedvalue;  /**< maximum possible allowed count values
                                     in tick                                */
    tpl_tick  ticksperbase;     /**< number of ticks required to
                                     reach a counter-specific (significant)
                                     unit                                   */
    tpl_tick  mincycle;         /**< smallest allowed value for the
                                     cycle-parameter of
                                     SetRelAlarm/SetAbsAlarm (only for
                                     systems with extended status)          */
};

/**
 * @typedef tpl_alarm_base
 *
 * This is an alias for the structure #ALARM_BASE_TYPE
 *
 * @see #ALARM_BASE_TYPE
 * @see #AlarmBaseType
 */
typedef struct ALARM_BASE_TYPE tpl_alarm_base;

/**
 * @typedef tpl_application_mode
 *
 * Identifies an application mode
 *
 * @see #AppModeType
 */
typedef u8 tpl_application_mode;

#endif /* TPL_OS_INTERNAL_TYPES_H */

/* End of file tpl_os_internal_type.h */
