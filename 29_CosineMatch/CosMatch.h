#ifndef _COS_MATCH_
#define _COS_MATCH_

#define MAX_SAMPLE_PTS	360          // maximum number of gesture sample data points
#define MAX_FEATURE_PTS 60          // number of feature sample points
#define MAX_TEMPLATES	3             // maximum number of registered gesture templates
#define NUM_REGISTRANTS 3           // required number of template registrant samples


#define LEN(x)			(sizeof(x)/sizeof(x[0]))
#define MIN(a, b)		( ((a) < (b)) ? (a) : (b) )
#define MAX(a, b)		( ((a) > (b)) ? (a) : (b) )
#define ABS(a)			( ((a) >= 0) ? (a) : -(a) )

#define PI				3.14159265358979323864F
#define ONE_8P8			256
#define ONE_4P12		4096
#define ONE_2P14		16384


typedef unsigned int uint32;
typedef short int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char uint8;
typedef short int8;

typedef struct
{
	/// @name "Gesture X"
	/// @section "Firmware Features"
	/// @category "Gesture recognizer"
	/// @description "The list of X coordinates of the points making up the saved user gesture.
	/// This is specific to each saved gesture."
	/// @tuning "This value is not manually tuned and is derived from the gesture being drawn."
	/// @default 0
	/// @public
  /*  /// @id x  */
	float x[MAX_FEATURE_PTS];

	/// @name "Gesture Y"
	/// @section "Firmware Features"
	/// @category "Gesture recognizer"
	/// @description "The list of Y coordinates of the points making up the saved user gesture.
	/// This is specific to each saved gesture."
	/// @tuning "This value is not manually tuned and is derived from the gesture being drawn."
	/// @default 0
	/// @public
  /*  /// @id y  */
	float y[MAX_FEATURE_PTS];

	/// @name "Scaling"
	/// @section "Firmware Features"
	/// @category "Gesture recognizer"
	/// @description "Computed by the firmware to scale a gesture into the maximum number of points allowed"
	/// @tuning "This is set by the firmware and should not changed by the user."
	/// @default 0
  /*  /// @id scaling   */
	float scaling;

	/// @name "Segments"
	/// @section "Firmware Features"
	/// @category "Gesture recognizer"
	/// @description "This is the number of strokes needed to draw the stored gesture."
	/// @tuning "Ensure this value matches the desired number of strokes needed to draw the stored gesture."
	/// @default 0
	/// @public
  /*  /// @id segments   */
	uint16 segments;

	/// @name "Valid"
	/// @section "Firmware Features"
	/// @category "Gesture recognizer"
	/// @description "This field identifies active gestures or allowed gestures from the stored set of gestures."
	/// @tuning "The host can set this field depending whether a particular gesture needs to be detected."
	/// @default 0
	/// @public
  /*  /// @id valid  */
	uint16 valid;
} grFeatures_t;

typedef struct
{
	/// @name "UDG Rotation Invariance"
	/// @section "Firmware Features"
	/// @category "User Defined Gestures"
	/// @description "Defines the maximum angle of rotation an UDG is allowed
	/// in both clockwise and counter-clockwise directions when compared against
	/// the corresponding version saved to the firmware."
	/// @tuning "Increase this value to allow for greater angular displacement relative to the stored gesture.
	/// Decrease this value to require stricter angular conformity."
	/// @units "radians"
	/// @max 7150
	/// @default 3575
	/// @public
	/// @id rotationInvariance
	float rotationInvariance;  // radians (0-pi)

	/// @name "UDG Scale Invariance"
	/// @section "Firmware Features"
	/// @category "User Defined Gestures"
	/// @description "Defines how large or small in size the performed gesture can be when
	/// compared against the corresponding saved version. The allowable size of the gesture ranges from
	/// 1/UDG Scale Invariance to UDG Scale Invariance times the original stored gesture size."
	/// @tuning "Increase this value to allow for greater size difference relative to the stored gesture.
	/// Decrease this value to require stricter conformity ."
	/// @default 8192
	/// @min 4096
	/// @public
	/// @id scaleInvariance
	float scaleInvariance;  // 1/scaleInvariance to scaleInvariance (1-16)

	/// @name "UDG Threshold Factor"
	/// @section "Firmware Features"
	/// @category "User Defined Gestures"
	/// @description "Defines the match error tolerance when a performed UDG is
	/// compared against its corresponding saved version"
	/// @tuning "This value is not typically altered. Increasing this value increases positive matches at the
	/// expense of increased false positives. Decreasing this value has the opposite effect."
	/// @units "radians"
	/// @max 28600
	/// @default 5720
	/// @public
	/// @id thresholdFactor
	float thresholdFactor;  // radians error (0-pi) to drive metric from 100 to 0

	/// @name "UDG Template Displacement"
	/// @section "Firmware Features"
	/// @category "User Defined Gestures"
	/// @description "Defines the number of coordinates allowed to be displaced
	/// when a UDG is performed in reference to a stored gesture."
	/// @tuning "This parameter is typically unchanged. When set too low, for example
	/// at 0, the firmware expects all points of the corresponding UDG are in place which
	/// can be difficult to achieve in reality. This may result in lower UDG sensitivity. On the
	/// contrary, increasing this parameter allows more buffer for error when performing an UDG."
	/// @default 2
	/// @public
	/// @id templateDisp
	uint16 templateDisp;  // number of template displacement points

	/// @name "UDG Template0"
	/// @section "Firmware Features"
	/// @category "User Defined Gestures"
	/// @description "This has one of the saved user defined gesture template."
	/// @tuning "Do not change this value"
	/// @default 0
	/// @id template0
	grFeatures_t template0;  // stored template

	/// @name "UDG Template1"
	/// @section "Firmware Features"
	/// @category "User Defined Gestures"
	/// @description "This has one of the saved user defined gesture template."
	/// @tuning "Do not change this value"
	/// @default 0
	/// @id template1
	grFeatures_t template1;  // stored template

	/// @name "UDG Template2"
	/// @section "Firmware Features"
	/// @category "User Defined Gestures"
	/// @description "This has one of the saved user defined gesture template."
	/// @tuning "Do not change this value"
	/// @default 0
	/// @id template2
	grFeatures_t template2;  // stored template
} grParams_t;

#endif // !_COS_MATCH_
