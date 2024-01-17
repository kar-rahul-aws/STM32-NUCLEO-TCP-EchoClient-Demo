/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

#include "FreeRTOS_IP.h"
#include "FreeRTOS_Firewall.h"

static portBASE_TYPE prvFirewallAddRuleCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static portBASE_TYPE prvFirewallListCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static portBASE_TYPE prvFirewallRemoveRuleCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/**
 * @brief Structure that defines the "firewall-add" command line command.
 */
static const CLI_Command_Definition_t xFirewallAddCommand =
{
    ( const char * const ) "firewall-add", /* The command string to type. */
    ( const char * const ) "firewall-add: Add a new firewall rule.\r\n",
    prvFirewallAddRuleCommandInterpreter, /* The interpreter function for the command. */
    6 /* No parameters are expected. */
};

/**
 * @brief Structure that defines the "firewall-list" command line command.
 */
static const CLI_Command_Definition_t xFirewallListCommand =
{
    ( const char * const ) "firewall-list", /* The command string to type. */
    ( const char * const ) "firewall-list: List active firewall rules.\r\n",
    prvFirewallListCommandInterpreter, /* The interpreter function for the command. */
    0 /* No parameters are expected. */
};

/**
 * @brief Structure that defines the "firewall-remove" command line command.
 */
static const CLI_Command_Definition_t xFirewallRemoveRuleCommand =
{
    ( const char * const ) "firewall-remove", /* The command string to type. */
    ( const char * const ) "firewall-remove: Remove active firewall rule by rule ID.\r\n",
    prvFirewallRemoveRuleCommandInterpreter, /* The interpreter function for the command. */
    1 /* No parameters are expected. */
};


/**
 * @brief Interpreter that handles the firewall-add command.
 */
static portBASE_TYPE prvFirewallAddRuleCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{

    portBASE_TYPE xReturn = pdFALSE;
    BaseType_t xRet = pdFALSE;

    xRet = xFirewallAddRule(((uint8_t *)pcCommandString) + strlen(xFirewallAddCommand.pcCommand));

    if(xRet == pdFALSE)
    {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Incorrect command\r\n");
    }
    else
    {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Firewall rule added\r\n");
    }
    return xReturn;
}

/**
 * @brief Interpreter that handles the firewall-list command.
 */
static portBASE_TYPE prvFirewallListCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    ( void ) pcCommandString;

    BaseType_t xRet = pdFALSE;

    xRet = xFirewallListRules((uint8_t *)pcWriteBuffer, xWriteBufferLen);

    if(xRet == pdFALSE)
    {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Internal error command (low buffer size)\r\n");
    }

    return pdFALSE;
}

/**
 * @brief Interpreter that handles the firewall-remove command.
 */
static portBASE_TYPE prvFirewallRemoveRuleCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    portBASE_TYPE xReturn = pdFALSE;
    BaseType_t xRet = pdFALSE;

    uintmax_t xRuleID = strtoumax((pcCommandString) + strlen(xFirewallRemoveRuleCommand.pcCommand), NULL, 10);
    if (!(xRuleID == UINTMAX_MAX && errno == ERANGE))
    {
    	xRet = xFirewallRemoveRule((uint32_t)xRuleID);
    }

    if(xRet == pdFALSE)
    {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Incorrect command\r\n");
    }
    else
    {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Firewall rule ID:%lu removed\r\n", (uint32_t) xRuleID);
    }
    return xReturn;
}

void vRegisterFirewallCommands( void )
{
    FreeRTOS_CLIRegisterCommand( &( xFirewallAddCommand ) );
    FreeRTOS_CLIRegisterCommand( &( xFirewallListCommand ) );
    FreeRTOS_CLIRegisterCommand( &( xFirewallRemoveRuleCommand ) );
}
