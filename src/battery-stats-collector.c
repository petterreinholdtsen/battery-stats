/*
    battery-stats-collector - collects statistics about battery performance
    Copyright (C) 2002 Karl E. Jørgensen <karl@jorgensen.com> 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <time.h>
#include <apm.h>
#include <syslog.h>

#ifdef HAVE_LIBACPI_H
#include <libacpi.h>
#endif

static const char *myname = "battery-stats-collector";
static const char *myversion = VERSION_STRING;

static int battery_num = 0;

static int do_syslog = 0;
#define COMPLAIN(loglevel, args...) if (do_syslog) syslog(loglevel, ## args); \
            else { fprintf(stderr,"%s: ", myname); fprintf(stderr, ##args); }

static struct option long_options[] = {
    { "output", required_argument, NULL, 'o' },
    { "interval", required_argument, NULL, 'i' },
    { "version", no_argument, NULL, 'V' },
    { "once", no_argument, NULL, '1' },
    { "help", no_argument, NULL, 'h' },
    { "syslog", no_argument, NULL, 's' },
    { "flush", required_argument, NULL, 'F' },
    { "ignore-missing-battery", no_argument, NULL, 'I' },
    { "battery-num", required_argument, NULL, 'b' },
    { NULL,	0, NULL, 0 }
};

static void apmdump(FILE *output, int ignore_missing_battery);
#ifdef WANT_ACPI
static void acpidump(FILE *output, const int ignore_missing_battery,
    global_t *libacpi_global);
#endif
void check_and_write_log_line(FILE *output, const int percentage,
    const int ac_state, const int remaining_time);
static void show_version(void);
static void show_usage(void);

static char *myname = "battery-stats-collector";
static char *myversion = "0.3.3";

static int battery_num = 0;

static int do_syslog = 0;
#define COMPLAIN(loglevel, args...) if (do_syslog) syslog(loglevel, ## args); \
			else { fprintf(stderr,"%s: ", myname); fprintf(stderr, ##args); }

int main(int argc, char **argv)
{
    int sample_interval_secs = 30;
    const char *stats_file_name = "/var/log/battery-stats";
    FILE *stats_file;
    int ignore_missing_battery = 0;
    long flush_interval = 1;
    long flush_count;

    while (1)
    {
	int option_index;
	int c;
	char *end;

	c = getopt_long(argc, argv, "o:i:Vh1sF:b:", long_options, &option_index);

	if (c == -1)
	    break;

	switch (c) {
	    case 'o':
        if (strcmp(optarg, "-") == 0)
          stats_file_name = 0;
        else
          stats_file_name = optarg;
		break;
		;;

	    case '1':
		sample_interval_secs = 0;
		break;

	    case 'i':
		sample_interval_secs = strtol(optarg, &end, 10);
		if (*end != 0)
		{
		    fprintf(stderr,"%s: Invalid number '%s'\n", myname, optarg);
		    exit(2);
		}

		if (sample_interval_secs < 0)
		{
		    fprintf(stderr,"%s: invalid negative sampling interval: '%s'\n",
				myname, optarg);
		    exit(2);
		}

		break;

	    case 'V':
		show_version();
		exit(0);
		break;

	    case 'h':
		show_version();
		show_usage();
		exit(0);
		break;

	    case 's':
		do_syslog = 1;
		break;

	    case 'I':
		ignore_missing_battery = 1;
		break;

	    case 'F':
		flush_interval = strtol(optarg, &end, 10);
		if (*end != 0)
		{
		    fprintf(stderr, "%s: Invalid flush interval '%s'\n", myname, optarg);
		    exit(2);
		}

		if (flush_interval < 0)
		{
		    fprintf(stderr, "%s: Invalid negative flush interval '%s'\n", myname, optarg);
		    exit(2);
		}
		break;

	    case 'b':
		battery_num = strtol(optarg, &end, 10);
		if (*end != 0)
		{
		    fprintf(stderr, "%s: Invalid battery number '%s'\n", myname, optarg);
		    exit(2);
		}

		if (battery_num < 0)
		{
		    fprintf(stderr, "%s: Invalid negative battery number '%s'\n", myname, optarg);
		    exit(2);
		}
		break;

	    default:
		/* getopt_long will already have complained on stderr ... */
		fprintf(stderr,"%s: Try %s --help for options\n", myname, myname);
		exit(2);
	}
    }

    if (optind < argc)
    {
	fprintf(stderr, "%s: Too many parameters\n", myname);

	exit(2);
    }

    if (do_syslog)
	openlog(myname, LOG_PID, LOG_DAEMON);

    if (stats_file_name) {
      stats_file = fopen(stats_file_name, "a");
    }
    else {
      stats_file = stdout;
    }
    if (stats_file == NULL)
    {
	COMPLAIN(LOG_ERR, "Cannot open output file '%s': %s\n",
	    stats_file_name, strerror(errno));

	exit(2);
    }

#ifdef WANT_ACPI
    // initialize libacpi and determine ACPI support
    global_t *libacpi_global = malloc (sizeof (global_t));
    int acpi_supported = check_acpi_support() == SUCCESS;
    COMPLAIN(LOG_INFO, "Using lib%s.\n", acpi_supported ? "acpi" : "apm");
    if(acpi_supported)
    {
      int retval = init_acpi_batt(libacpi_global);
      switch(retval)
      {
	case SUCCESS:
	  COMPLAIN(LOG_INFO, "Number of batteries: %i.\n",
	      libacpi_global->batt_count);
	  if (libacpi_global->batt_count > 1) {
	      COMPLAIN(LOG_INFO,
		       "Reading info from battery %d only.\n", battery_num);
	  }
	  break;
	case NOT_SUPPORTED:
	  COMPLAIN(LOG_WARNING, "You have more than %i batteries.  "
	      "Reading info from first battery only anyway.\n", MAX_ITEMS);
	  break;
	case ALLOC_ERR:
	  COMPLAIN(LOG_ERR, "libacpi allocation error.\n");
	  exit(2);
	case ITEM_EXCEED:
	  COMPLAIN(LOG_WARNING, "You have more than %i batteries.  "
	      "Reading info from first battery only anyway.\n", MAX_ITEMS);
	  break;
	default:
	  COMPLAIN(LOG_ERR, "init_acpi_batt() returned unknown value: %i\n",
	      retval);
	  exit(2);
      }
      retval = init_acpi_acadapt(libacpi_global);
      switch(retval)
      {
	case SUCCESS:
	  break;
	case NOT_SUPPORTED:
	  COMPLAIN(LOG_ERR, "init_acpi_acadapt() returned NOT_SUPPORTED.\n");
	  exit(2);
	case ALLOC_ERR:
	  COMPLAIN(LOG_ERR, "libacpi allocation error.\n");
	  exit(2);
      }
    }
#endif
    flush_count = 0;
    while (1)
    {
#ifdef WANT_ACPI
	if(acpi_supported) {
		acpidump(stats_file, ignore_missing_battery, libacpi_global);
	} else {
		apmdump(stats_file, ignore_missing_battery);
	}
#endif

#ifndef WANT_ACPI
	apmdump(stats_file, ignore_missing_battery);
#endif

	flush_count++;
	if (flush_count >= flush_interval)
	{
	    fflush(stats_file);
	    flush_count = 0;
	}
	if (sample_interval_secs == 0)
	    break;

	sleep(sample_interval_secs);
    }

#ifdef WANT_ACPI
    free(libacpi_global);
#endif
    fclose(stats_file);
    if (do_syslog)
	closelog();

    exit(0);
}

static void show_version(void)
{
    printf("%s version %s, Copyright 2002 Karl E. Jørgensen\n", myname, myversion);
    printf("%s comes with ABSOLUTELY NO WARRANTY.\n", myname);
    puts("This is free software, and you are welcome to redistribute it under");
    puts("conditions, read the license for details.");
}

static void show_usage(void)
{
    struct option *o = long_options;

    puts("Usage:");
    printf("\t%s", myname);

    for (o = long_options; o->name != NULL; o++)
	printf(" [ --%s | -%c ]", o->name, o->val);

    puts("");
}


#ifdef DEBUG
static void apm_fulldump(struct apm_info *ai);
#endif

static void apmdump(FILE *output, int ignore_missing_battery)
{
    struct apm_info ai;
    int rc;

    rc = apm_read(&ai);

    if (rc != 0)
    {
	COMPLAIN(LOG_ERR,"apm_read failed with error code %d\n", rc);
	return;
    }

#ifdef DEBUG
    apm_fulldump(&ai);
#endif

    if (ai.ac_line_status == AC_LINE_STATUS_UNKNOWN)
    {
	COMPLAIN(LOG_ERR, "AC Line status unknown\n");
	return;
    }

    if (ai.battery_flags & BATTERY_FLAGS_ABSENT)
    {
	if (!ignore_missing_battery) {
	    COMPLAIN(LOG_WARNING, "Battery absent\n");
       } 
	return;
    }

    if (ai.battery_percentage == BATTERY_PERCENTAGE_UNKNOWN)
    {
	COMPLAIN(LOG_ERR, "Battery present, but percentage unknown!?\n");
	return;
    }

    check_and_write_log_line(output, ai.battery_percentage,
	(ai.ac_line_status == AC_LINE_STATUS_OFF) ? 0
	: ((ai.ac_line_status == AC_LINE_STATUS_ON) ? 1 : 2),
	ai.battery_time);
}

#ifdef DEBUG
static void apm_fulldump(struct apm_info *ai)
{
    printf("AC-Line-status:");
    switch (ai->ac_line_status)
    {
	case AC_LINE_STATUS_OFF:	printf(" off"); break;
	case AC_LINE_STATUS_ON:		printf(" on"); break;
	case AC_LINE_STATUS_BACKUP:	printf(" backup"); break;
	default:			printf(" unknown");
    }

    printf(" Battery-Status:");
    switch (ai->battery_status)
    {
	case BATTERY_STATUS_HIGH:	printf(" high"); break;
	case BATTERY_STATUS_LOW:	printf(" low"); break;
	case BATTERY_STATUS_CRITICAL:	printf(" critical"); break;
	case BATTERY_STATUS_CHARGING:	printf(" charging"); break;
	case BATTERY_STATUS_ABSENT:	printf(" absent"); break;
	default:			printf(" unknown"); break;
    }

    printf(" Battery-Flags: %02x=", ai->battery_flags);
    if (ai->battery_flags & BATTERY_FLAGS_HIGH) printf("[HIGH]");
    if (ai->battery_flags & BATTERY_FLAGS_LOW) printf("[LOW]");
    if (ai->battery_flags & BATTERY_FLAGS_CRITICAL) printf("[CRITICAL]");
    if (ai->battery_flags & BATTERY_FLAGS_CHARGING) printf("[CHARGING]");
    if (ai->battery_flags & BATTERY_FLAGS_ABSENT) printf("[ABSENT]");

    printf(" Percentage: ");
    if (ai->battery_percentage == BATTERY_PERCENTAGE_UNKNOWN)
	printf("unknown");
    else
	printf("%3d%%", ai->battery_percentage);

    printf(" Minutes: ");
    if (ai->battery_time == BATTERY_TIME_UNKNOWN)
	printf("unknown");
    else
	printf("%d", ai->battery_time);

    printf(" Using-Minutes: %02x\n", ai->using_minutes);
}
#endif

#ifdef WANT_ACPI
static void acpidump(FILE *output, const int ignore_missing_battery,
    global_t *libacpi_global)
{
    int rc;

    if (battery_num >= libacpi_global->batt_count) {
	COMPLAIN(LOG_ERR,"Battery %d doesn't exist. The number of batteries is: %d.\n", battery_num, libacpi_global->batt_count);
	return;
    }
    rc = read_acpi_batt(battery_num);
    if (rc != SUCCESS)
    {
	COMPLAIN(LOG_ERR,"read_acpi_batt(%d) failed with error code %d\n", battery_num, rc);
	return;
    }
    if (!batteries[battery_num].present)
    {
	if (!ignore_missing_battery) {
	    COMPLAIN(LOG_WARNING, "Battery %d absent\n", battery_num);
        }
	return;
    }

    read_acpi_acstate(libacpi_global);
    if (libacpi_global->adapt.ac_state == P_ERR)
    {
	COMPLAIN(LOG_ERR, "AC Line status unknown\n");
	return;
    }

    check_and_write_log_line(output, batteries[battery_num].percentage,
	(libacpi_global->adapt.ac_state == P_BATT) ? 2 :
	 ((libacpi_global->adapt.ac_state == P_AC) ? 1 : 0),
	batteries[battery_num].remaining_time);
}
#endif

void check_and_write_log_line(FILE *output, const int percentage,
    const int ac_state, const int remaining_time)
{
    time_t theTime;
    time(&theTime);
    if (theTime == -1)
    {
	COMPLAIN(LOG_ERR, "Cannot get current time !?\n");
	return;
    }

    struct tm *tm = gmtime(&theTime);

    if (percentage > 100)
    {
	COMPLAIN(LOG_ERR, "Battery percentage > 100 (%d) !?\n",
		percentage);
	return;
    }

    if (percentage < 0)
    {
	COMPLAIN(LOG_ERR, "Negative battery percentage (%d) !?\n",
		percentage);
	return;
    }

    fprintf(output, "%ld %d %d %04d/%02d/%02d %02d:%02d:%02d %d\n",
	    theTime, percentage, ac_state,
	    tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
	      tm->tm_hour, tm->tm_min, tm->tm_sec,
	    remaining_time);
}

