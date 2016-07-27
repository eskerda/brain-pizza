#ifndef STROKE_HPP
#define STROKE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <deque>
#include <cmath>

#ifndef M_PI
  #define M_PI 3.14159265358979323846f
#endif

float myRand(float low, float high);

////////////////////////////////////////////////////////////
/// \brief A graphical stroke.
////////////////////////////////////////////////////////////
class Stroke : public sf::Drawable
{
	public:
		enum SweepStyle
		{
			Soft = 0,
			Softer,
			Sharp
		};

		enum DrawMode
		{
			Solid = 0,
			Textured,
			Monochrome,
			WireFrame
		};

	private:
		////////////////////////////////////////////////////////////
		/// \brief Define a joint of the stroke, with all its data members.
		///
		////////////////////////////////////////////////////////////
		struct Joint
		{
			Joint();
			sf::Vector2f	position;
			sf::Vector2f	offset;
			float			birth_time;
			float			thickness;
			float			angle;
			sf::Vector2f	upper_point;
			sf::Vector2f	lower_point;
			sf::Color		inner_color;
			sf::Color		outer_color;
			float			thickness_scale;
		};

		////////////////////////////////////////////////////////////
		// Member data
		////////////////////////////////////////////////////////////
		// Bones
		std::deque<Joint>			joints;				///< The joints of the stroke
		float						time;				///< The lifetime elapsed, used with joints lifetime

		// Body structure
		unsigned int				max_joints;			///< Maximum joints the stroke can have
		float						max_length;			///< Maximum length the stroke can have
		float						min_segment_length; ///< Minimum length between each segments
		float						joint_lifetime;		///< Maximum time the joints can stay
		sf::Vector2f				joint_linear_speed; ///< Linear speed the joints move

		// Stroke option
		float						thickness;			///< Global thickness
		sf::Vector2f				random_thickness;	///< Random thickness applyed when adding joint
		float						start_thickness;	///< Thickness at the start of the stroke
		float						end_thickness;		///< Thickness at the end of the stroke
		float						stroke_offset;		///< Position of the separation between start and end
		float						start_sweep;		///< Amplitude of the sweep effect at the start
		float						end_sweep;			///< Amplitude of the sweep effect at the end
		SweepStyle					start_sweep_style;	///< Effect used at the start of the stroke
		SweepStyle					end_sweep_style;	///< Effect used at the end of the stroke
		float						stippling;			///< Amplitude of the stippling effect
		float						shaking;			///< Amplitude of the shaking effect
		bool						loop;				///< Loop the stroke by joining first and last joint

		// Appearance
		DrawMode					draw_mode;			///< Drawing mode used to render the stroke
		sf::Color					start_inner_color;	///< Inner color at the start of the stroke (used by monochrome mode)
		sf::Color					start_outer_color;	///< Outer color at the start of the stroke
		sf::Color					end_inner_color;	///< Inner color at the end of the stroke
		sf::Color					end_outer_color;	///< Outer color at the end of the stroke

		// Texture
		sf::Texture                 texture;				///< Image used in Textured mode
		sf::IntRect					sub_rect;			///< Region drawn from the image
		bool						symetric;			///< Image drawn symetricaly around the stroke
		float						texture_offset;		///< Offset of the texture in the stroke
		float						texture_pan;		///< Pan (number of bloc) the texture will expand on

		// Length
		bool						is_length_compiled;	///< Flag for lengh computation
		float						length;				///< Total length of the stroke

		// Bonding Box
		bool						is_bounding_box_compiled;	///< Flag for bounding box computation
		sf::FloatRect				bounding_box;				///< Bounding box of the stroke

		// State
		bool						is_compiled;		///< Flag for stroke computation

		////////////////////////////////////////////////////////////
		/// \brief Draw the object to a render target
		///
		/// Render the stroke according to the drawing mode choosed.
		/// The function will compile the stroke and its length if needed.
		///
		/// \param target   Render target
		/// \param renderer Renderer providing low-level rendering commands
		///
		////////////////////////////////////////////////////////////
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		////////////////////////////////////////////////////////////
		/// \brief Draw the object to a render target
		///
		/// DrawSolid draw the stroke with a solid style, meaning
		/// the stroke is composed with gradiant polygones.
		///
		/// \param target   Render target
		/// \param renderer Renderer providing low-level rendering commands
		///
		////////////////////////////////////////////////////////////
		void DrawSolid(sf::RenderTarget& target, sf::RenderStates states) const;

		////////////////////////////////////////////////////////////
		/// \brief Draw a bloc in the DrawSolid function
		///
		/// A 'bloc' is one of the polygone drawn to form the final stroke.
		///
		/// \param target   Render target
		/// \param renderer Renderer providing low-level rendering commands
		/// \param current_joint Identifier of the current joint
		/// \param previous_joint Identifier of the previous joint
		///
		////////////////////////////////////////////////////////////
		void DrawSolidBloc(sf::RenderTarget & target, sf::RenderStates states, unsigned int current_joint, unsigned int previous_joint) const;
		////////////////////////////////////////////////////////////
		/// \brief Draw the object to a render target
		///
		/// DrawTexture draw the stroke using the image provided with
		/// the SetImage() function. If no image is assigned, it call
		/// the DrawMonochrome instead.
		///
		/// \param target   Render target
		/// \param renderer Renderer providing low-level rendering commands
		///
		////////////////////////////////////////////////////////////
		void DrawTexture(sf::RenderTarget & target, sf::RenderStates states) const;

		////////////////////////////////////////////////////////////
		/// \brief Draw a bloc in the DrawTexture function
		///
		/// A 'bloc' is one of the polygone drawn to form the final stroke.
		///
		/// \param target   Render target
		/// \param renderer Renderer providing low-level rendering commands
		/// \param TopLeft Top-left coordinate of the bloc
		/// \param TopRight Top-right coordinate of the bloc
		/// \param CenterLeft Left coordinates of the stroke main line
		/// \param CenterRight Right coordinates of the stroke main line
		/// \param BottomLeft Bottom-left coordinates of the bloc
		/// \param BottomRight Bottom-right coordinates of the bloc
		/// \param TexCoords Coordinates of the region used as texture on the current image
		///
		////////////////////////////////////////////////////////////
		void DrawTexturedBloc(sf::RenderTarget & target, sf::RenderStates states, const sf::Vector2f & TopLeft, const sf::Vector2f & TopRight, const sf::Vector2f & CenterLeft, const sf::Vector2f & CenterRight, const sf::Vector2f & BottomLeft, const sf::Vector2f & BottomRight, const sf::FloatRect & TexCoords) const;

		////////////////////////////////////////////////////////////
		/// \brief Draw the object to a render target
		///
		/// DrawMonochrome draw the stroke only using the color provided
		/// with the SetColor function.
		///
		/// \param target   Render target
		/// \param renderer Renderer providing low-level rendering commands
		///
		////////////////////////////////////////////////////////////
		void DrawMonochrome(sf::RenderTarget& target, sf::RenderStates states) const;

		////////////////////////////////////////////////////////////
		/// \brief Draw a bloc in the DrawMonochrome function
		///
		/// A 'bloc' is one of the polygone drawn to form the final stroke.
		///
		/// \param target   Render target
		/// \param renderer Renderer providing low-level rendering commands
		/// \param current_joint Identifier of the current joint
		/// \param previous_joint Identifier of the previous joint
		///
		////////////////////////////////////////////////////////////
		void DrawMonochromeBloc(sf::RenderTarget & target, sf::RenderStates states, unsigned int current_joint, unsigned int previous_joint) const;

		////////////////////////////////////////////////////////////
		/// \brief Draw the object to a render target
		///
		/// DrawWireframe draw the stroke with a wired style. The main line
		/// of the stroke is drawn in white, while the other lines are in gray.
		/// It is mainly made for debugging and benchmark usage.
		///
		/// \param target   Render target
		/// \param renderer Renderer providing low-level rendering commands
		///
		////////////////////////////////////////////////////////////
		void DrawWireframe(sf::RenderTarget& target, sf::RenderStates states) const;

		////////////////////////////////////////////////////////////
		/// \brief Compile the stroke
		///
		/// This function precomputes all the internal parameters
		/// needed to properly render the stroke (external points, thickness).
		///
		////////////////////////////////////////////////////////////
		bool Compile();

		////////////////////////////////////////////////////////////
		/// \brief Compute the shaking effect
		///
		/// This function precomputes the joints offset due to the
		/// shaking effect.
		/// It is called only if the shaking is not at 0.
		///
		////////////////////////////////////////////////////////////
		void ComputeOffset();

		////////////////////////////////////////////////////////////
		/// \brief Compute the bounding box
		///
		/// This function precomputes the bounding box of the stroke.
		/// It is called only if a bounding box is asked using
		/// the GetBoundingBox() function.
		///
		////////////////////////////////////////////////////////////
		void CompileBoundingBox();

		////////////////////////////////////////////////////////////
		/// \brief Compute the length
		///
		/// This function precomputes the total length of the stroke,
		/// in pixels. It is called only if joints are added, moved or
		/// removed.
		///
		////////////////////////////////////////////////////////////
		void CompileLength();

		////////////////////////////////////////////////////////////
		/// \brief Compute the thickness
		///
		/// This function precomputes the joints thickness regarding
		/// the thickness parameters.
		///
		/// \param alpha	The alpha value of the current joint,
		///					meaning the length of the joint divised by
		///					the total length. (Its position on the stroke.)
		/// \param offset	The offset of the sweep.
		/// \param t		The base joint thickness multiplyed by
		///					the global thickness.
		///
		////////////////////////////////////////////////////////////
		float ComputeThickness(float alpha, float offset, float t);

		////////////////////////////////////////////////////////////
		/// \brief Compute the sweep
		///
		/// This function precomputes the stroke sweep effect for
		/// the current joint, regarding the desired sweep style.
		///
		/// \param sweep_style	The wanted sweep style to apply
		/// \param a			The current thickness
		///
		/// /return The new thickness regarding the sweep.
		////////////////////////////////////////////////////////////
		float ApplySweep(int sweep_style, float a);

		////////////////////////////////////////////////////////////
		/// \brief Check if a value is in range
		///
		/// This function ensure the value of a desired variable is
		/// in its correct range. If the value is out of the range,
		/// it round the value to the min or max value.
		///
		/// \param value	The initial value
		/// \param min		The minimum wanted value
		/// \param max		The maximum wanted value
		///
		/// \return	The correct value regarding its range.
		///
		////////////////////////////////////////////////////////////
		inline float ToRange(float value, float min = 0.f, float max = 1.f);

		/**********************************************************/
		// Some mathematical functions
		/**********************************************************/
		////////////////////////////////////////////////////////////
		/// \brief Angle convertion in degree
		///
		/// \param angle The angle in radian
		///
		/// \return	The angle in degree
		///
		////////////////////////////////////////////////////////////
		inline float ToDeg(float angle);

		////////////////////////////////////////////////////////////
		/// \brief Angle convertion in radian
		///
		/// \param angle The angle in degree
		///
		/// \return	The angle in radian
		///
		////////////////////////////////////////////////////////////
		inline float ToRad(float angle);

		////////////////////////////////////////////////////////////
		/// \brief Angle were the segment P1P2 point to
		///
		/// \param P1 First point of the segment
		/// \param P2 Second point of the segment
		///
		/// \return	The angle of the segment, in the range [-180, 180]
		///
		////////////////////////////////////////////////////////////
		inline float Angle(const sf::Vector2f & P1, const sf::Vector2f & P2);

		////////////////////////////////////////////////////////////
		/// \brief Distance between two points
		///
		/// \param P1 First point
		/// \param P2 Second point
		///
		/// \return	The distance between each point
		///
		////////////////////////////////////////////////////////////
		inline float Distance(const sf::Vector2f & P1, const sf::Vector2f & P2);

		////////////////////////////////////////////////////////////
		/// \brief Vector from an angle and length
		///
		/// \param angle The angle of the vector
		/// \param length The length of the vector
		///
		/// \return	The desired vector
		///
		////////////////////////////////////////////////////////////
		inline sf::Vector2f ToVector(float angle, float length);

		////////////////////////////////////////////////////////////
		/// \brief Sign of a number
		///
		/// \param value The number we want the sign from
		///
		/// \return	1 if positive, -1 if negative. value = 0 is considered positive.
		///
		////////////////////////////////////////////////////////////
		inline float Sign(float value);

		////////////////////////////////////////////////////////////
		/// \brief Norm of a number
		///
		/// \param value The number we want the norm from
		///
		/// \return	The norm of the number
		///
		////////////////////////////////////////////////////////////
		inline float Norm(float value);

		////////////////////////////////////////////////////////////
		/// \brief Interpolate between two colors
		///
		/// \param color1	First color
		/// \param color2	Second color
		/// \param alpha	The alpha value, in the range [0, 1]
		///
		/// \return	The interpolated color
		///
		////////////////////////////////////////////////////////////
		inline sf::Color ColorInterpolation(const sf::Color & color1, const sf::Color & color2, float alpha);

	public:
		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		/// Creates an empty stroke (no joints).
		///
		////////////////////////////////////////////////////////////
		Stroke();

		////////////////////////////////////////////////////////////
		/// \brief Get the linear speed applied to the points
		///
		/// \return	Linear speed, in pixels/sec
		///
		/// \see SetJointLinearSpeed
		///
		////////////////////////////////////////////////////////////
		const sf::Vector2f & GetJointLinearSpeed() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the maximum joints the stroke can contain
		///
		/// \return	Maximum joints of the stroke
		///
		/// \see SetMaxJoints
		///
		////////////////////////////////////////////////////////////
		unsigned int GetMaxJoints() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the maximum length the stroke can have
		///
		/// \return	Maximum length of the stroke, in pixels
		///
		/// \see SetMaxLength
		///
		////////////////////////////////////////////////////////////
		float GetMaxLength() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the minimum length between each joint
		///
		/// \return	Minimum length, in pixels
		///
		/// \see SetMinSegmentLength
		///
		////////////////////////////////////////////////////////////
		float GetMinSegmentLength() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the lifetime of each new joint
		///
		/// \return	Lifetime, in seconds
		///
		/// \see SetJointLifeTime
		///
		////////////////////////////////////////////////////////////
		float GetJointLifetime() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the global thickness of the stroke
		///
		/// \return	Global thickness, un pixels
		///
		/// \see SetThickness
		///
		////////////////////////////////////////////////////////////
		float GetThickness() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the quotien of the minimum thickness that can
		/// be randomly generated
		///
		/// \return	Minimum thickness
		///
		/// \see SetMaxRandomThickness
		///
		////////////////////////////////////////////////////////////
		float GetMaxRandomThickness() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the quotien of the maximum thickness that can
		/// be randomly generated
		///
		/// \return	Maximum thickness
		///
		/// \see SetMinRandomThickness
		///
		////////////////////////////////////////////////////////////
		float GetMinRandomThickness() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the quotien of the thickness at the start of the stroke.
		///
		/// \return	Start thickness, in range [0, 1]
		///
		/// \see SetStartThickness
		///
		////////////////////////////////////////////////////////////
		float GetStartThickness() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the quotien of the thickness at the end of the stroke.
		///
		/// \return	End thickness, in range [0,1]
		///
		/// \see SetEndThickness
		///
		////////////////////////////////////////////////////////////
		float GetEndThickness() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the offset separating the start from the end of the stroke.
		///
		/// \return	Offset, in range [-1, 1]
		///
		/// \see SetOffset
		///
		////////////////////////////////////////////////////////////
		float GetOffset() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the quotien of the sweep at the start of the stroke.
		///
		/// \return	Start sweep, in range [0, 1]
		///
		/// \see SetStartSweep
		///
		////////////////////////////////////////////////////////////
		float GetStartSweep() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the quotien of the sweep at the end of the stroke.
		///
		/// \return	End sweep, in range [0, 1]
		///
		/// \see SetEndSweep
		///
		////////////////////////////////////////////////////////////
		float GetEndSweep() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the style of the sweep at the start of the stroke.
		///
		/// \return	Sweep style
		///
		/// \see SetStartSweepStyle
		///
		////////////////////////////////////////////////////////////
		SweepStyle GetStartSweepStyle() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the style of the sweep at the end of the stroke.
		///
		/// \return	Sweep style
		///
		/// \see SetEndSweepStyle
		///
		////////////////////////////////////////////////////////////
		SweepStyle GetEndSweepStyle() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the quotien of the stippling effect
		///
		/// \return	Stippling, in range [0, 1]
		///
		/// \see SetStippling
		///
		////////////////////////////////////////////////////////////
		float GetStippling() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the amplitude of the shaking effect
		///
		/// \return	Shaking amplitude, in pixels
		///
		/// \see SetShaking
		///
		////////////////////////////////////////////////////////////
		float GetShaking() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the looping flag
		///
		/// \return	True if the stroke loop
		///
		/// \see SetLoop
		///
		////////////////////////////////////////////////////////////
		bool IsLooping() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the inner color at the start of the stroke
		///
		/// \return	Start inner color
		///
		/// \see SetStartInnerColor, SetInnerColor
		///
		////////////////////////////////////////////////////////////
		const sf::Color & GetStartInnerColor() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the outer color at the start of the stroke
		///
		/// \return	Start outer color
		///
		/// \see SetStartOuterColor, SetOuterColor
		///
		////////////////////////////////////////////////////////////
		const sf::Color & GetStartOuterColor() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the inner color at the end of the stroke
		///
		/// \return	End inner color
		///
		/// \see SetEndInnerColor, SetInnerColor
		///
		////////////////////////////////////////////////////////////
		const sf::Color & GetEndInnerColor() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the outer color at the end of the stroke
		///
		/// \return	End outer color
		///
		/// \see SetEndOuterColor, SetOuterColor
		///
		////////////////////////////////////////////////////////////
		const sf::Color & GetEndOuterColor() const;

		////////////////////////////////////////////////////////////
		/// \brief Get a pointer to the image used with the Textured style
		///
		/// If the sprite has no source image, or if the image
		/// doesn't exist anymore, a NULL pointer is returned.
		/// The returned pointer is const, which means that you can't
		/// modify the image when you retrieve it with this function.
		///
		/// \return	Pointer to the stroke's image
		///
		/// \see SetImage
		///
		////////////////////////////////////////////////////////////
		const sf::Texture & GetTexture() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the region of the image displayed by the stroke
		///
		/// \return	Rectangle defining the region of the image
		///
		/// \see SetSubRect
		///
		////////////////////////////////////////////////////////////
		const sf::IntRect & GetSubRect() const;

		////////////////////////////////////////////////////////////
		/// \brief Indicate if the texture is symetric to the stroke
		///
		/// \return	True is the texture is symetric
		///
		/// \see SetSymetric
		///
		////////////////////////////////////////////////////////////
		bool IsSymetric() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the texture offset of the stroke
		///
		/// \return	Texture offset
		///
		/// \see SetTextureOffset
		///
		////////////////////////////////////////////////////////////
		float GetTextureOffset() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the texture pan of the stroke
		///
		/// \return	Texture pan
		///
		/// \see SetTexturePan
		///
		////////////////////////////////////////////////////////////
		float GetTexturePan() const;

		////////////////////////////////////////////////////////////
		/// \brief Get total length of the stroke
		///
		/// \return Total length
		///
		////////////////////////////////////////////////////////////
		float GetLength();

		////////////////////////////////////////////////////////////
		/// \brief Get the bounding box of the stroke
		///
		/// The bounding box is a rectangle incorporating all the points
		/// created to draw the stroke. It is usefull for collision test.
		/// For example, a stroke from wich the bounding box is out of the
		/// current view isn't visible on the screen, and thus it don't need
		/// to be drawn.
		///
		/// \return	Rectangle defining the bounding box of the stroke
		///
		////////////////////////////////////////////////////////////
		const sf::FloatRect & GetBoundingBox();

		////////////////////////////////////////////////////////////
		/// \brief Indicate if the stroke will be appearent if we draw it
		///
		/// This function indicate if the stroke have the minimum requierements
		/// to be visible on screen. The requierements are:
		/// - at least 3 joints
		/// - a thickness different from 0
		///
		/// \return	True if the stroke has enough joints
		///
		////////////////////////////////////////////////////////////
		bool IsDrawable() const;

		// Modificateurs de l'objet
		////////////////////////////////////////////////////////////
		/// \brief Set the joint's linear speed
		///
		/// The linear speed allow the joints to move themselves. It requier
		/// that the stroke is made dynamic and keep its joints free from
		/// custom data, meaning the stroke isn't cleared at each frame. The
		/// speed is expressed in pixel per second on each axis.
		///
		/// Default value : 0 for each Axis
		///
		///	\param x Horizontal velocity
		/// \param y Vertical velocity
		///
		/// \see GetJointLinearSpeed
		///
		////////////////////////////////////////////////////////////
		void SetJointLinearSpeed(float x, float y);

		////////////////////////////////////////////////////////////
		/// \brief Set the joint's linear speed
		///
		/// The linear speed allow the joints to move themselves. It requier
		/// that the stroke is made dynamic and keep its joints free from
		/// custom data, meaning the stroke isn't cleared at each frame. The
		/// speed is expressed in pixel per second on each axis.
		///
		/// Default value : 0 for each Axis
		///
		///	\param linear_speed Linear speed in pixel/sec
		///
		/// \see GetJointLinearSpeed
		///
		////////////////////////////////////////////////////////////
		void SetJointLinearSpeed(const sf::Vector2f & linear_speed);

		////////////////////////////////////////////////////////////
		/// \brief Set the maximum joints the stroke can have
		///
		/// Each stroke is composed of a number of joints. Having a
		/// restriction on their number can avoid having a stroke
		/// infinitely growing. It is a way to constraint the length
		/// of a stroke.
		///
		/// Default value : 0
		///
		/// Warning : if you want to use a dynamic stroke, be sure to
		/// set a maximum value to at least one of the following functions:
		/// - SetMaxJoints
		/// - SetMaxLength
		/// - SetJointLifetime
		///
		///	\param max_joints The maximum joints the stroke can have
		///
		/// \see GetMaxJoints, SetMaxJoints, SetMaxLength, SetJointLifetime
		///
		////////////////////////////////////////////////////////////
		void SetMaxJoints(unsigned int max_joints);

		////////////////////////////////////////////////////////////
		/// \brief Set the maximum length the stroke can have
		///
		/// Stroke don't have limits in length. This function allow
		/// you to set a limit to the length. The length is basically
		/// the sum of the distances between each joints, not the distance
		/// between the first and last joint.
		/// If the length of the stroke became too long when adding new joints
		/// with \a AddJoint function, the \ Calculate function will remove
		/// the last joints of the list until the length get back smaller than
		/// the maximum length.
		/// A maximum length of 0 let the stroke grow infinitely.
		///
		/// Warning : if you want to use a dynamic stroke, be sure to
		/// set a maximum value to at least one of the following functions:
		/// - SetMaxJoints
		/// - SetMaxLength
		/// - SetJointLifetime
		///
		/// Default value : 0
		///
		///	\param max_length The maximum length the stroke can have, in pixels
		///
		/// \see GetMaxLength, SetMaxJoints, SetMaxLength, SetJointLifetime
		///
		////////////////////////////////////////////////////////////
		void SetMaxLength(float max_length);

		////////////////////////////////////////////////////////////
		/// \brief Set the minimum length between each joint
		///
		/// Having too few joints give to your stroke a sad polygonal look,
		/// but too much can be a threat to your program performances.
		/// The minimum length ensure a new joint placed with the /a AddJoint
		/// function will have a minimum length with the previous one to
		/// be added. If it is too close to the previous joint, the new joint
		/// won't be added. If it is too far, intermediate joints will be added
		/// forming a straight line.
		/// A minimum length of 0 allow ANY new joints to be added.
		///
		/// Default value : 0
		///
		///	\param min_segment_length The minimum length between each joint, in pixels
		///
		/// \see GetMinSegmentLength
		///
		////////////////////////////////////////////////////////////
		void SetMinSegmentLength(float min_segment_length);

		////////////////////////////////////////////////////////////
		/// \brief Set the lifetime of each joints
		///
		/// This function give a maximum lifetime to your joints. Too
		/// old joints will be removed by the \a Calculate function.
		/// It is a cool way to create organic strokes depleting in time.
		/// It is also a way to avoid infinitely growing strokes.
		///
		/// Warning : if you want to use a dynamic stroke, be sure to
		/// set a maximum value to at least one of the following functions:
		/// - SetMaxJoints
		/// - SetMaxLength
		/// - SetJointLifetime
		///
		/// Default value : 0
		///
		///	\param joint_lifetime The lifetime of each joint, in seconds
		///
		/// \see GetJointLifetime
		///
		////////////////////////////////////////////////////////////
		void SetJointLifetime(float joint_lifetime);

		////////////////////////////////////////////////////////////
		/// \brief Set the global thickness of the stroke
		///
		/// The global thickness is the main thickness of the stroke. It
		/// prevail over the overthickness, meaning a stroke with a thickness
		/// of 0 won't be drawn.
		///
		/// Default value : 3 pixels
		///
		///	\param thickness The global thickness, in pixels
		///
		/// \see GetThickness
		///
		////////////////////////////////////////////////////////////
		void SetThickness(float thickness);

		////////////////////////////////////////////////////////////
		/// \brief Set the range of random thickness
		///
		/// Each joint possess a thickness of it's own. Settings a thickness
		/// with this function will give a randomly choosed thickness to each
		/// new joint added with the \a AddJoint function.
		/// The random thickness take multiples of the global thickness.
		/// Example :
		/// \code
		/// mu_stroke.SetThickness(30.f);
		/// my_stroke.SetRandomThickness(0.5f, 1.5f);
		/// \endcode
		/// With this code, each new joint will receive a thickness
		/// between 15 and 45 pixels.
		///
		/// Default value : 1 to min and max
		///
		///	\param min Minimum possible value
		/// \param max Maximim possible value
		///
		/// \see GetMaxRandomThickness, GetMinRandomThickness
		///
		////////////////////////////////////////////////////////////
		void SetRandomThickness(float min, float max);

		////////////////////////////////////////////////////////////
		/// \brief Set the thickness at the start of the stroke
		///
		/// The stroke have a starting and ending thickness. It allow it
		/// to have a smoothed shape, defined by both the start/end thickness,
		/// sweep and sweep style.
		/// A thickness of 0 will make the stroke tapered at the start,
		/// a thickness of 1 will make it squared.
		///
		/// Default value : 1
		///
		///	\param start_thickness Thickness at the start of the stroke, in range [0, 1]
		///
		/// \see GetStartThickness
		///
		////////////////////////////////////////////////////////////
		void SetStartThickness(float start_thickness);

		////////////////////////////////////////////////////////////
		/// \brief Set the thickness at the end of the stroke
		///
		/// The stroke have a starting and ending thickness. It allow it
		/// to have a smoothed shape, defined by both the start/end thickness,
		/// sweep and sweep style.
		/// A thickness of 0 will make the stroke tapered at the end,
		/// a thickness of 1 will make it squared.
		///
		/// Default value : 1
		///
		///	\param end_thickness Thickness at the end of the stroke, in range [0, 1]
		///
		/// \see GetEndThickness
		///
		////////////////////////////////////////////////////////////
		void SetEndThickness(float end_thickness);

		////////////////////////////////////////////////////////////
		/// \brief Set the offset of the stroke
		///
		/// Having a stroke with tapered start and end give it a more
		/// organic look. The offset allow you to move the separation
		/// between each part of the stroke (start and end) along the strok.
		/// An offset of 1 will move it to the start of the stroke,
		/// an offset of -1 will move it at the end and an offset of
		/// 0 keep it at midway of the stroke.
		/// The offset is useless if neither of the start or end of the stroke
		/// have a thickness lower that 1.
		///
		/// Default value : 0
		///
		///	\param stroke_offset Offset of the stroke, in range [-1, 1]
		///
		/// \see GetOffset
		///
		////////////////////////////////////////////////////////////
		void SetOffset(float stroke_offset);

		////////////////////////////////////////////////////////////
		/// \brief Set the sweep of the stroke
		///
		/// The default tapered stroke take a diamond shape. The sweep
		/// allow to give your stroke a more swift and smooth shape.
		/// A sweep of 0 will keep the stroke a diamond shape, while
		/// a sweep of 1 will give it the full sweep shape defined with
		/// the sweep style.
		///
		/// SetSweep function set the sweep for both start and end of the stroke.
		///
		/// Default value : 0
		///
		///	\param sweep Sweep of the stroke, in range [0, 1]
		///
		/// \see GetStartSweep, GetEndSweep, SetStartSweep, SetEndSweep, SetSweepStyle
		///
		////////////////////////////////////////////////////////////
		void SetSweep(float sweep);

		////////////////////////////////////////////////////////////
		/// \brief Set the start sweep of the stroke
		///
		/// The default tapered stroke take a diamond shape. The sweep
		/// allow to give your stroke a more swift and smooth shape.
		/// A sweep of 0 will keep the stroke a diamond shape, while
		/// a sweep of 1 will give it the full sweep shape defined with
		/// the sweep style.
		///
		/// Default value : 0
		///
		///	\param start_sweep Sweep of the start of the stroke, in range [0, 1]
		///
		/// \see GetStartSweep, SetSweepStyle
		///
		////////////////////////////////////////////////////////////
		void SetStartSweep(float start_sweep);

		////////////////////////////////////////////////////////////
		/// \brief Set the end sweep of the stroke
		///
		/// The default tapered stroke take a diamond shape. The sweep
		/// allow to give your stroke a more swift and smooth shape.
		/// A sweep of 0 will keep the stroke a diamond shape, while
		/// a sweep of 1 will give it the full sweep shape defined with
		/// the sweep style.
		///
		/// Default value : 0
		///
		///	\param end_sweep Sweep of the end of the stroke, in range [0, 1]
		///
		/// \see GetEndSweep, SetSweepStyle
		///
		////////////////////////////////////////////////////////////
		void SetEndSweep(float end_sweep);

		////////////////////////////////////////////////////////////
		/// \brief Set the sweep style of the stroke
		///
		/// The sweep style is the shape the tapered stroke will take
		/// with a sweep greater than 0.
		///
		/// The sweep style allow the following style :
		///	- Sin: the stroke will be lightly rounded
		///	- Sin2: the stroke will be more rounded,
		///	- Sin3: the stroke will be even more rounded,
		///	- SquareSin : the stroke will have a piercing look, rounding
		///				  the more it get closer to the offset position
		///
		/// SetSweepStyle set the sweep style for both start and end of the stroke.
		///
		/// Default value : e_Sin for both side
		///
		///	\param sweep_style Sweep style of the stroke
		///
		/// \see SweepStyle, GetStartSweepStyle, GetEndSweepStyle
		///
		////////////////////////////////////////////////////////////
		void SetSweepStyle(SweepStyle sweep_style);

		////////////////////////////////////////////////////////////
		/// \brief Set the sweep style at the start of the stroke
		///
		/// The sweep style is the shape the tapered stroke will take
		/// with a sweep greater than 0.
		///
		/// The sweep style allow the following style :
		///	- Sin: the stroke will be lightly rounded
		///	- Sin2: the stroke will be more rounded,
		///	- Sin3: the stroke will be even more rounded,
		///	- SquareSin : the stroke will have a piercing look, rounding
		///				  the more it get closer to the offset position
		///
		/// Default value : e_Sin
		///
		///	\param start_sweep_style Sweep style at the start of the stroke
		///
		/// \see SweepStyle, GetStartSweepStyle
		///
		////////////////////////////////////////////////////////////
		void SetStartSweepStyle(SweepStyle start_sweep_style);

		////////////////////////////////////////////////////////////
		/// \brief Set the sweep style at the end of the stroke
		///
		/// The sweep style is the shape the tapered stroke will take
		/// with a sweep greater than 0.
		///
		/// The sweep style allow the following style :
		///	- Sin: the stroke will be lightly rounded
		///	- Sin2: the stroke will be more rounded,
		///	- Sin3: the stroke will be even more rounded,
		///	- SquareSin : the stroke will have a piercing look, rounding
		///				  the more it get closer to the offset position
		///
		/// Default value : e_Sin
		///
		///	\param end_sweep_style Sweep style at the end of the stroke
		///
		/// \see SweepStyle, GetEndSweepStyle
		///
		////////////////////////////////////////////////////////////
		void SetEndSweepStyle(SweepStyle end_sweep_style);

		////////////////////////////////////////////////////////////
		/// \brief Set the stippling of the stroke
		///
		/// This function allow you to set a stippling quotien for
		/// your stroke. The stippling will give a dotted look to your
		/// stroke.
		///
		/// A stippling of 0 disable the stippling effect while
		/// a stippling of 1 will make the stroke non-displayable.
		///
		/// Default value : 0
		///
		///	\param stippling Stippling of the stroke, in range [0, 1]
		///
		/// \see GetStippling
		///
		////////////////////////////////////////////////////////////
		void SetStippling(float stippling);

		////////////////////////////////////////////////////////////
		/// \brief Set the shaking amplitude of the stroke
		///
		/// The shaking is a simple distortion of the stroke. With
		/// static stroke, the shaking will give a distorded look at
		/// your stroke, with dynamic stroke, it will make the stroke
		/// shake over the time.
		///
		/// Default value : 0
		///
		///	\param shaking Shaking amplitude of the stroke, in pixels
		///
		/// \see GetShaking
		///
		////////////////////////////////////////////////////////////
		void SetShaking(float shaking);

		////////////////////////////////////////////////////////////
		/// \brief Set the shaking amplitude of the stroke
		///
		/// Looping the stroke can be usefull to draw closed path like
		/// circles or squares. Looping the stroke join the first and last
		/// joint, computing their angles accordingly.
		///
		/// Default value : false
		///
		///	\param loop Set to true if you need to make the stroke looping
		///
		/// \see GetShaking
		///
		////////////////////////////////////////////////////////////
		void SetLoop(bool loop);

		////////////////////////////////////////////////////////////
		/// \brief Set the drawing mode for the stroke
		///
		/// The drawing mode allow you to choose between several
		/// behavior to render the stroke. The drawing mode can take
		/// one of those modes :
		/// - Solid:		the basic mode, draw the stroke with gradiant colors.
		/// - Textured:		draw the stroke using an image (assigned with \a SetImage)
		///					If no image is assigned, the stroke is drawn with
		///					monochrome mode.
		/// - Monochrome:	draw the stroke with a uni color, wich is set
		///					with \a SetColor
		/// - Wireframe:	draw the stroke with a wired style. The main line
		///					of the stroke is drawn in white and the other lines
		///					are drawn in gray.
		///
		/// Default value : Solid
		///
		///	\param draw_mode Drawing mode for the stroke
		///
		/// \see DrawMode
		///
		////////////////////////////////////////////////////////////
		void SetDrawMode(DrawMode draw_mode);

		////////////////////////////////////////////////////////////
		/// \brief Set the inner color of the stroke
		///
		/// The stroke allow to use different color for both in the stroke
		/// and out the stroke. It allow to create gradiant between the center
		/// and extremities of the stroke.
		///
		/// For both inner and outer color, you can also set the color at
		/// the start and the end of the stroke.
		///
		/// SetInnerColor set the inner color for both start and end of
		/// the stroke.
		///
		/// Default value : sf::Color(255, 255, 255)
		///
		///	\param inner_color Inner color of the stroke
		///
		/// \see GetStartInnerColor, GetEndInnerColor
		///
		////////////////////////////////////////////////////////////
		void SetInnerColor(const sf::Color & inner_color);

		////////////////////////////////////////////////////////////
		/// \brief Set the outer color of the stroke
		///
		/// The stroke allow to use different color for both in the stroke
		/// and out the stroke. It allow to create gradiant between the center
		/// and extremities of the stroke.
		///
		/// For both inner and outer color, you can also set the color at
		/// the start and the end of the stroke.
		///
		/// SetOuterColor set the outer color for both start and end of
		/// the stroke.
		///
		/// Default value : sf::Color(255, 255, 255)
		///
		///	\param outer_color Outer color of the stroke
		///
		/// \see GetStartOuterColor, GetEndOuterColor
		///
		////////////////////////////////////////////////////////////
		void SetOuterColor(const sf::Color & outer_color);

		////////////////////////////////////////////////////////////
		/// \brief Set the inner color at the start of the stroke
		///
		/// The stroke allow to use different color for both in the stroke
		/// and out the stroke. It allow to create gradiant between the center
		/// and extremities of the stroke.
		///
		/// For both inner and outer color, you can also set the color at
		/// the start and the end of the stroke.
		///
		/// Default value : sf::Color(255, 255, 255)
		///
		///	\param start_inner_color Inner color at the start of the stroke
		///
		/// \see GetStartInnerColor
		///
		////////////////////////////////////////////////////////////
		void SetStartInnerColor(const sf::Color & start_inner_color);

		////////////////////////////////////////////////////////////
		/// \brief Set the outer color at the start of the stroke
		///
		/// The stroke allow to use different color for both in the stroke
		/// and out the stroke. It allow to create gradiant between the center
		/// and extremities of the stroke.
		///
		/// For both inner and outer color, you can also set the color at
		/// the start and the end of the stroke.
		///
		/// Default value : sf::Color(255, 255, 255)
		///
		///	\param start_outer_color Outer color at the start of the stroke
		///
		/// \see GetStartOuterColor
		///
		////////////////////////////////////////////////////////////
		void SetStartOuterColor(const sf::Color & start_outer_color);

		////////////////////////////////////////////////////////////
		/// \brief Set the inner color at the end of the stroke
		///
		/// The stroke allow to use different color for both in the stroke
		/// and out the stroke. It allow to create gradiant between the center
		/// and extremities of the stroke.
		///
		/// For both inner and outer color, you can also set the color at
		/// the start and the end of the stroke.
		///
		/// Default value : sf::Color(255, 255, 255)
		///
		///	\param end_inner_color Outer color at the end of the stroke
		///
		/// \see GetStartInnerColor
		///
		////////////////////////////////////////////////////////////
		void SetEndInnerColor(const sf::Color & end_inner_color);

		////////////////////////////////////////////////////////////
		/// \brief Set the outer color at the end of the stroke
		///
		/// The stroke allow to use different color for both in the stroke
		/// and out the stroke. It allow to create gradiant between the center
		/// and extremities of the stroke.
		///
		/// For both inner and outer color, you can also set the color at
		/// the start and the end of the stroke.
		///
		/// Default value : sf::Color(255, 255, 255)
		///
		///	\param end_outer_color Outer color at the end of the stroke
		///
		/// \see GetEndOuterColor
		///
		////////////////////////////////////////////////////////////
		void SetEndOuterColor(const sf::Color & end_outer_color);

		////////////////////////////////////////////////////////////
		/// \brief Set the image used has texture of the stroke
		///
		/// A colored stroke is good, a textured one is (often) better!
		/// This function allow you to set an sf::Image as a texture for
		/// the stroke. Adding an image automaticaly turn the drawing mode
		/// to Textured. The image respond to the symetric, texture offset
		/// and texture pan member data. If an invalid image is passed,
		/// the stroke will be rendered in Monochrome mode.
		///
		/// Default value : 0 (no image)
		///
		///	\param image Image used has texture
		/// \param adjust_to_new_size Set to True to adapt the sub rect to the size of the image
		///
		/// \see GetImage, SetSubRect
		///
		////////////////////////////////////////////////////////////
		void SetTexture(const sf::Texture & texture, bool adjust_to_new_size = false);

		////////////////////////////////////////////////////////////
		/// \brief Set the part of the image that the stroke will display
		///
		/// The sub-rectangle is useful when you don't want to display
		/// the whole image, but rather a part of it.
		/// By default, the sub-rectangle covers the entire image.
		///
		/// \param sub_rect Rectangle defining the region of the image to display
		///
		/// \see GetSubRect, SetImage
		///
		////////////////////////////////////////////////////////////
		void SetSubRect(const sf::IntRect & sub_rect);

		////////////////////////////////////////////////////////////
		/// \brief Set symetry of the stroke
		///
		/// By setting the symetry, you can choose whether the texture
		/// is drawn once or twice on each sub-part of the stroke.
		///
		/// \param symetric Set to True to use the symetry
		///
		/// \see GetSubRect, SetImage
		///
		////////////////////////////////////////////////////////////
		void SetSymetric(bool symetric);

		////////////////////////////////////////////////////////////
		/// \brief Set the texture offset
		///
		/// The texture offset allow you to move the texture along
		/// the stroke. It can take positive or negative value.
		/// One stroke unit match to 'texture_pan' blocs.
		///
		/// texture_move = texture_offset * texture_pan
		///
		/// \param texture_offset The offset of the texture
		///
		/// \see GetTextureOffset
		///
		////////////////////////////////////////////////////////////
		void SetTextureOffset(float texture_offset);

		////////////////////////////////////////////////////////////
		/// \brief Move the texture
		///
		/// The texture offset allow you to move the texture along
		/// the stroke. It can take positive or negative value.
		/// One stroke unit match to 'texture_pan' blocs.
		///
		/// texture_move = texture_offset * texture_pan
		///
		/// MoveTexture add the value to the existing offset
		///
		/// \param texture_offset The offset of the texture
		///
		/// \see GetTextureOffset
		///
		////////////////////////////////////////////////////////////
		void MoveTexture(float texture_offset);

		////////////////////////////////////////////////////////////
		/// \brief Set the texture pan
		///
		/// The texture pan strech the texture over 'texture_pan' bloc.
		/// The minimum pan is 1, meaning the texture will repeat at each
		/// bloc. A pan of 3 will repeat the texture each 3 blocs.
		///
		/// \param texture_pan The pan of the texture
		///
		/// \see GetTexturePan
		///
		////////////////////////////////////////////////////////////
		void SetTexturePan(float texture_pan);

		////////////////////////////////////////////////////////////
		/// \brief Clear the joint array
		///
		/// Remove all the joints from the stroke.
		///
		////////////////////////////////////////////////////////////
		void Clear();

		////////////////////////////////////////////////////////////
		/// \brief Add a joint to the stroke
		///
		/// Add a joint to the stroke, giving its position and if
		/// a precompilation of the stroke is wanted.
		///
		/// \param x Position of the joint on X Axis (relative position)
		/// \param y Position of the joint on Y Axis (relative position)
		/// \param precompile Set to true if a precompilation is needed
		///
		/// \return True if the joint has been successfully added
		///
		////////////////////////////////////////////////////////////
		bool AddJoint(float x, float y, bool precompile = false);

		////////////////////////////////////////////////////////////
		/// \brief Add a joint to the stroke
		///
		/// Add a joint to the stroke, giving its position and if
		/// a precompilation of the stroke is wanted.
		///
		/// \param position Position of the joint (relative position)
		/// \param precompile Set to true if a precompilation is needed
		///
		/// \return True if the joint has been successfully added
		///
		////////////////////////////////////////////////////////////
		bool AddJoint(const sf::Vector2f & position, bool precompile = false);

		////////////////////////////////////////////////////////////
		/// \brief Add a joint to the stroke
		///
		/// Add a joint to the stroke, giving its position and if
		/// a precompilation of the stroke is wanted.
		///
		/// With the thickness scale, the randomness given by
		/// the random Thickness parameter will be ignored.
		///
		/// \param x Position of the joint on X Axis (relative position)
		/// \param y Position of the joint on Y Axis (relative position)
		/// \param thickness_scale Thickness of the joint
		/// \param precompile Set to true if a precompilation is needed
		///
		/// \return True if the joint has been successfully added
		///
		////////////////////////////////////////////////////////////
		bool AddJoint(float x, float y, float thickness_scale, bool precompile = false);

		////////////////////////////////////////////////////////////
		/// \brief Add a joint to the stroke
		///
		/// Add a joint to the stroke, giving its position and if
		/// a precompilation of the stroke is wanted.
		///
		/// With the thickness scale, the randomness given by
		/// the random Thickness parameter will be ignored.
		///
		/// \param position Position of the joint (relative position)
		/// \param thickness_scale Thickness of the joint
		/// \param precompile Set to true if a precompilation is needed
		///
		/// \return True if the joint has been successfully added
		///
		////////////////////////////////////////////////////////////
		bool AddJoint(const sf::Vector2f & position, float thickness_scale, bool precompile = false);

		////////////////////////////////////////////////////////////
		/// \brief Add a joint to the stroke
		///
		/// Calculate must be called if you want to make your stroke
		/// dynamic. It calculate the position of the joints according
		/// to the linear speed and delete useless joints regarding
		/// maximum length, maximum joint number and joint lifetime
		/// parameters.
		///
		/// \param time_interval Duration of the frame
		///
		////////////////////////////////////////////////////////////
		void Calculate(float time_interval);
};

#endif // STROKE_HPP

////////////////////////////////////////////////////////////
/// \class Stroke
///
///	The Stroke class allow to draw real-time animated strokes.
///	It give access to severals stroke parameter, different drawing
///	styles and is optimized wether the stroke is static (meaning all
/// its joints/points won't change) or dynamic.
///
/// Stroke is a graphical class, meaning it is better to 'link' it
/// to other data structure, such as a list or vector of points if
/// you want a better control over it, for example, handling collision
/// with your stroke.
///
/// Stroke inherit sf::Drawable, meaning all transformation properties
/// of the drawable works. It has a heavy interface as it is wanted to
/// be complete and suitable for a large panel of usage.
///
/// Static stroke usage example :
/// \code
/// Stroke my_stroke;
/// // A stroke need at least 3 joints to be displayed.
/// my_stroke.AddJoint(40, 40);
/// my_stroke.AddJoint(200, 50);
/// // Set the precompile parameter to true if you want to
/// // precompile your stroke while loading your application.
/// my_stroke.AddJoint(230, 400, true);
///
/// while(true)
/// {
///		App.Clear();
///		App.Draw(my_stroke);
///		App.Display();
/// }
/// \endcode
///
/// Dynamic stroke usage example :
/// \code
/// Stroke my_stroke;
/// // We add some speed to the joints to make the stroke grow upward.
/// my_stroke.SetJointLinearSpeed(0, -200);
///
/// while(true)
/// {
///		// Precompiling is useless because the stroke will change each frame.
///		my_stroke.AddJoint(400, 300);
///
///		// We need to compute the joints to delete
///		// useless joints and move existing ones.
///		my_stroke.Calculate(App.GetFrameTime());
///
///		App.Clear();
///		App.Draw(my_stroke);
///		App.Display();
/// }
/// \endcode
////////////////////////////////////////////////////////////
