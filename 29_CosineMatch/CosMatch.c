#include <stdio.h>
#include <stdlib.h>
#include "CosMatch.h"
#include "math.h"

grParams_t cfg;

static float range(float *values, uint16 length)
{
	//range Calculate range of values
	//
	// values - vector of values
	// length - length of x (included for C implementation)

	// calculate range;

	uint16 i;
	float minValue = values[0];
	float maxValue = values[0];

	for (i = 1; i < length; i++)
	{
		if (minValue > values[i])
		{
			minValue = values[i];
		}
		if (maxValue < values[i])
		{
			maxValue = values[i];
		}
	}

	return(1 + maxValue - minValue);
}

static void angleFeatures(float *angles, float *xfeatures, float *yfeatures, uint16 length)
{
	//angleFeatures Process angle features
	//
	//xfeatures - x position in mm's offset by COM
	//yfeatures - y position in mm's offset by COM
	//length - length of features

	uint16 i;

	//determine angle between points
	for (i = 0; i < length - 1; i++)
	{
		float xdif = (float)(xfeatures[i + 1] - xfeatures[i]) / ONE_8P8;  // row vector of first differences (size = length-1)
		float ydif = (float)(yfeatures[i + 1] - yfeatures[i]) / ONE_8P8;  // row vector of first differences (size = length-1)
		angles[i] = atan2f(ydif, xdif);  // row vector of relative angles in radians (size = length-1)
	}
	angles[length - 1] = 0.0f;  // no information exists at this end of the data, so set to zero (size = length)
}


static float matchFeatures(grFeatures_t *tfeatures, grFeatures_t *ufeatures)
{
	//matchFeatures Match user gesture against template
	//
	// tfeatures - template features structure
	// ufeatures - user gesture features structure
	//   features data structures have fields of:
	//         x - x position in mm's offset by COM
	//         y - y position in mm's offset by COM
	//         segments - total number of segments (strokes) in gesture
	//         scaling - recognition cost scaling factor
	//         valid - uint16ean to indicate validity of values
	// metric - feature match metric [-100 100] (>0 = match)
	// x (debug only use) - rotated x position in mm's offset by COM
	// y (debug only use) - rotated y position in mm's offset by COM

	uint16 i;
	float a, b;
	float theta;
	float x[MAX_FEATURE_PTS], y[MAX_FEATURE_PTS];
	float tangles[MAX_FEATURE_PTS], uangles[MAX_FEATURE_PTS];
	float xscale, yscale;
	float ang[MAX_FEATURE_PTS];
	float angErr = 0.0f;
	float vtt = 0.0f;
	float vuu = 0.0f;
	float vtu = 0.0f;
	float metric = 0.0f;  // no match

	printf("Enter %s\n", __func__);
	// determine angle between vectors
	a = 0.0f;
	b = 0.0f;
	for (i = 0; i < MAX_FEATURE_PTS; i++)
	{
		a += (float)tfeatures->x[i] * (float)ufeatures->x[i] + (float)tfeatures->y[i] * (float)ufeatures->y[i];  // dot products
		b += (float)tfeatures->x[i] * (float)ufeatures->y[i] - (float)tfeatures->y[i] * (float)ufeatures->x[i];  // dot products
	}
	theta = atan2f(b, a);
	printf("a = %.2f, b = %.2f, theta = %.2f\n", a, b, theta);

	// rotate template by angle to coincide
	for (i = 0; i < MAX_FEATURE_PTS; i++)
	{
		x[i] = (float)(cosf(theta) * (float)tfeatures->x[i] - sinf(theta) * (float)tfeatures->y[i]);  // row vectors
		y[i] = (float)(sinf(theta) * (float)tfeatures->x[i] + cosf(theta) * (float)tfeatures->y[i]);  // row vectors
	}

	// determine scale
	xscale = (float)range(ufeatures->x, MAX_FEATURE_PTS) / (float)range(x, MAX_FEATURE_PTS);  // range is strictly positive
	yscale = (float)range(ufeatures->y, MAX_FEATURE_PTS) / (float)range(y, MAX_FEATURE_PTS);  // range is strictly positive
	printf("xscale = %.2f, yscale = %.2f\n", xscale, yscale);

	// check scale
	a = (float)cfg.scaleInvariance / ONE_4P12;
	if (((MIN(xscale, yscale)*a) < 1.0f) || (MAX(xscale, yscale) > a))
	{
		printf("Return metric = %f\n", metric);
		return metric;
	}

	// calculate angles
	angleFeatures(tangles, x, y, MAX_FEATURE_PTS);
	angleFeatures(uangles, ufeatures->x, ufeatures->y, MAX_FEATURE_PTS);

	// create angle difference
	for (i = 0; i < MAX_FEATURE_PTS; i++)
	{
		ang[i] = tangles[i] - uangles[i];  // row vector

		if (ang[i] > PI)
		{
			ang[i] = ang[i] - 2.0f * PI;
		}
		if (ang[i] < -PI)
		{
			ang[i] = ang[i] + 2.0f * PI;
		}
		angErr += ang[i] * ang[i];
	}

	// limit difference to [-pi, pi]
	//ind = find(ang > pi);  // indices where ang value is > pi
	//ang(ind) = 2*pi - ang(ind);  // shift values at those indices to [0, pi] range

	// average angular distance error
	angErr = sqrtf(angErr) / MAX_FEATURE_PTS;
	printf("angErr = %f\n", angErr);

	// calculate match metric
	for (i = 0; i < MAX_FEATURE_PTS; i++)
	{
		vtt += (float)x[i] * (float)x[i] + (float)y[i] * (float)y[i];  // dot product
		vuu += (float)ufeatures->x[i] * (float)ufeatures->x[i] + (float)ufeatures->y[i] * (float)ufeatures->y[i];  // dot product
		vtu += (float)x[i] * (float)ufeatures->x[i] + (float)y[i] * (float)ufeatures->y[i];  // dot product
	}
	printf("vtt = %f, vuu = %f\n", vtt, vuu);

	if ((vtt != 0.0f) && (vuu != 0.0f))
	{
		float max_scaling, tmp, posErr;
		float rotErr, strErr, totalErr;

		// position angular error
		tmp = vtu / sqrtf(vtt) / sqrtf(vuu);
		posErr = acosf(MAX(-1.0f, MIN(1.0f, tmp)));

		// rotation angular error - hits mismatch at additional 1x the rotation invariance
		a = (float)MAX(1, cfg.thresholdFactor) / ONE_2P14;
		b = (float)MAX(1, cfg.rotationInvariance) / ONE_2P14;
		max_scaling = (float)MAX(tfeatures->scaling, ufeatures->scaling) / ONE_4P12;
		rotErr = a / max_scaling / b * MAX(0.0f, (ABS(theta) - b));

		strErr = a / max_scaling * 0.01f * (float)ABS((int)(tfeatures->segments) - (int)(ufeatures->segments));

		// total error
		totalErr = MIN(200.0f, 100.0f / a * max_scaling * (angErr + posErr + rotErr + strErr));

		// map to metric reange of [-100 100]
		metric = 100.0f - totalErr;
	}

	return metric;
}

static initCfg()
{
	uint8 i; 

	cfg.scaleInvariance = 8192;
	cfg.thresholdFactor = 5720;
	cfg.rotationInvariance = 3575;
	cfg.templateDisp = 2;
	cfg.template0.segments = 1;
	cfg.template0.scaling = 1.5f;
	cfg.template1.segments = 1;
	cfg.template1.scaling = 1.5;
	cfg.template2.segments = 1;
	cfg.template2.scaling = 1.5;

	for (i = 0; i < MAX_FEATURE_PTS; i++)
	{
		cfg.template0.x[i] = (float)i;
		cfg.template0.y[i] = (float)i;
	}

	for (i = 0; i < MAX_FEATURE_PTS; i++)
	{
		cfg.template1.x[i] = (float)(2 * i);
		cfg.template1.y[i] = (float)(2 * i);
	}

	for (i = MAX_FEATURE_PTS; i > 0; i--)
	{
		cfg.template2.x[i] = (float)i;
		cfg.template2.y[i] = (float)(i * i);
	}
}

void main()
{
	float matchRet, ret = 0.0f;
	initCfg();
	matchRet = matchFeatures(&cfg.template0, &cfg.template0);
	printf("matchRet : %f\n", matchRet);

	//ret = atan2f(1.0, 1.0);
	//printf("ret = %f\n", ret);
	printf("ret = %f\n", ret);



	system("pause");
}