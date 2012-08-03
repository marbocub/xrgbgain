#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <malloc.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

typedef struct {
	int	red;
	int	green;
	int	blue;
} rgb_int;

void usage (char *argv0)
{
	printf ("usage: %s [OPTIONS]\n"
		"\n"
		" -1, --rgamma <value>         set Red   Gamma value\n"
		" -2, --ggamma <value>         set Green Gamma value\n"
		" -3, --bgamma <value>         set Blue  Gamma value\n"
		" -R, --rcontrast <0-100>      set Red   Contrast rate\n"
		" -G, --gcontrast <0-100>      set Green Contrast rate\n"
		" -B, --bcontrast <0-100>      set Blue  Contrast rate\n"
		" -r, --rbrightness <0-100>    set Red   Brightness rate\n"
		" -g, --gbrightness <0-100>    set Green Brightness rate\n"
		" -b, --bbrightness <0-100>    set Blue  Brightness rate\n"
		, argv0);
}

int main(int argc, char **argv)
{
	Display		*display;
	int		screen;

	int		i;
	int		ramp_size;
	rgb_int		brightness, contrast;
	unsigned short	*red, *green, *blue;
	double		level;
	XF86VidModeGamma gamma;

	int		c;
	char		*argv0, *e;
	float		gamma_value;
	int		parcent_value;
	int		option_index = 0;
	struct option	long_options[] = {
		{"rgamma",      required_argument, NULL, '1'},
		{"ggamma",      required_argument, NULL, '2'},
		{"bgamma",      required_argument, NULL, '3'},
		{"rbrightness", required_argument, NULL, 'r'},
		{"gbrightness", required_argument, NULL, 'g'},
		{"bbrightness", required_argument, NULL, 'b'},
		{"rcontrast",   required_argument, NULL, 'R'},
		{"gcontrast",   required_argument, NULL, 'G'},
		{"bcontrast",   required_argument, NULL, 'B'},
		{0, 0, 0, 0}
	};


	/* default values */
	gamma.red        = 1.0;
	gamma.green      = 1.0;
	gamma.blue       = 1.0;
	brightness.red   = 0;
	brightness.green = 0;
	brightness.blue  = 0;
	contrast.red     = 100;
	contrast.green   = 100;
	contrast.blue    = 100;

	/* parse command-line options */
	argv0 = argv[0];
	while (1) {
		c = getopt_long (argc, argv, "1:2:3:r:g:b:R:G:B:", long_options, &option_index);
		if (c == -1)
			break;
		switch (c) {
		case '1':
		case '2':
		case '3':
			gamma_value = strtof(optarg, &e);
			if (*e != '\0') {
				printf ("%s: '%s' is invalid value.\n",
					argv0, optarg);
				usage (argv0);
				exit (0);
			}
			switch (c) {
			case '1':
				gamma.red = gamma_value;
				break;
			case '2':
				gamma.green = gamma_value;
				break;
			case '3':
				gamma.blue = gamma_value;
				break;
			}
			break;

		case 'r':
		case 'g':
		case 'b':
		case 'R':
		case 'G':
		case 'B':
			parcent_value = strtol(optarg, &e, 0);
			if (*e != '\0') {
				printf ("%s: '%s' is invalid parcentage.\n",
					argv0, optarg);
				usage (argv0);
				exit (0);
			}
			switch (c) {
			case 'r':
				brightness.red   = parcent_value;
				break;
			case 'g':
				brightness.green = parcent_value;
				break;
			case 'b':
				brightness.blue  = parcent_value;
				break;
			case 'R':
				contrast.red   = parcent_value;
				break;
			case 'G':
				contrast.green = parcent_value;
				break;
			case 'B':
				contrast.blue  = parcent_value;
				break;
			}
			break;

		case '?':
			usage (argv0);
			exit (0);
			break;
		defaut:
			usage (argv0);
			exit (0);
			break;
		}
	}
	if (optind < argc) {
		usage (argv0);
		exit (0);
	}
	printf ("\tgamma\tbright\tcontrast\n");
	printf ("Red  \t%5.2f\t%4d %%\t  %4d %%\n", gamma.red,   brightness.red,   contrast.red);
	printf ("Green\t%5.2f\t%4d %%\t  %4d %%\n", gamma.green, brightness.green, contrast.green);
	printf ("Blue \t%5.2f\t%4d %%\t  %4d %%\n", gamma.blue,  brightness.blue,  contrast.blue);


	/* open X device */
	if ((display = XOpenDisplay (NULL)) == NULL) {
		fprintf (stderr, "Unable to open X display.\n");
		return 1;
	}
	screen = DefaultScreen (display);

	/* allocate LUT */
	if (! XF86VidModeGetGammaRampSize (display, screen, &ramp_size)) {
		fprintf (stderr, "Unable to query XF86VidMode gamma ramp size.\n");
		XCloseDisplay (display);
		return 2;
	}
	red   = (unsigned short *) malloc (sizeof (unsigned short) * ramp_size);
	green = (unsigned short *) malloc (sizeof (unsigned short) * ramp_size);
	blue  = (unsigned short *) malloc (sizeof (unsigned short) * ramp_size);
	if (red == NULL || green == NULL || blue == NULL) {
		fprintf (stderr, "Unable to allocate memory.\n");
		free (red);
		free (green);
		free (blue);
		XCloseDisplay (display);
		return 3;
	}

	/* setup LUT */
	XF86VidModeSetGamma (display, screen, &gamma);
	XF86VidModeGetGammaRamp (display, screen, ramp_size, red, green, blue);
	for (i=0; i<ramp_size; i++) {
		level = red[i]   * (contrast.red   - brightness.red)  /100.0 + 65535*brightness.red  /100.0;
		if (level > 65535)  level = 65535;
		else if (level < 0) level = 0;
		red[i] = level;

		level = green[i] * (contrast.green - brightness.green)/100.0 + 65535*brightness.green/100.0;
		if (level > 65535)  level = 65535;
		else if (level < 0) level = 0;
		green[i] = level;

		level = blue[i]  * (contrast.blue  - brightness.blue) /100.0 + 65535*brightness.blue /100.0;
		if (level > 65535)  level = 65535;
		else if (level < 0) level = 0;
		blue[i] = level;
	}
	XF86VidModeSetGammaRamp (display, screen, ramp_size, red, green, blue);

	/* closing */
	free (red);
	free (green);
	free (blue);
	XCloseDisplay (display);

	return 0;
}
