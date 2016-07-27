#ifdef _MSC_VER
	#pragma warning(disable: 4244) //Cast int to float
#endif

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Stroke.hpp"
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#ifdef WIN32
	#include <windows.h>
#endif

float myRand(float low, float high)
{
	return static_cast<float>(std::rand()) / RAND_MAX * (high - low) + low;
}

// Constructeur
////////////////////////////////////////////////////////////
Stroke::Joint::Joint() :
birth_time(0),
thickness_scale(1),
angle(0),
thickness(0)
{
}

////////////////////////////////////////////////////////////
Stroke::Stroke() :
time(0),
max_joints(0),
max_length(0),
min_segment_length(0),
joint_lifetime(0),
thickness(3),
random_thickness(1, 1),
start_thickness(1),
end_thickness(1),
stroke_offset(0),
start_sweep(0),
end_sweep(0),
start_sweep_style(Soft),
end_sweep_style(Soft),
stippling(0),
shaking(0),
loop(false),
draw_mode(Solid),
start_inner_color(sf::Color::White),
start_outer_color(sf::Color::White),
end_inner_color(sf::Color::White),
end_outer_color(sf::Color::White),
sub_rect(0,0,1,1),
symetric(true),
texture_offset(0),
texture_pan(1),
is_length_compiled(false),
length(0),
is_bounding_box_compiled(false),
is_compiled(false)
{
}

// Accesseurs
////////////////////////////////////////////////////////////
const sf::Vector2f & Stroke::GetJointLinearSpeed() const
{
	return joint_linear_speed;
}

////////////////////////////////////////////////////////////
unsigned int Stroke::GetMaxJoints() const
{
	return max_joints;
}

////////////////////////////////////////////////////////////
float Stroke::GetMaxLength() const
{
	return max_length;
}

////////////////////////////////////////////////////////////
float Stroke::GetMinSegmentLength() const
{
	return min_segment_length;
}

////////////////////////////////////////////////////////////
float Stroke::GetJointLifetime() const
{
	return joint_lifetime;
}

////////////////////////////////////////////////////////////
float Stroke::GetThickness() const
{
	return thickness;
}

////////////////////////////////////////////////////////////
float Stroke::GetMaxRandomThickness() const
{
	return random_thickness.y;
}

////////////////////////////////////////////////////////////
float Stroke::GetMinRandomThickness() const
{
	return random_thickness.x;
}

////////////////////////////////////////////////////////////
float Stroke::GetStartThickness() const
{
	return start_thickness;
}

////////////////////////////////////////////////////////////
float Stroke::GetEndThickness() const
{
	return end_thickness;
}

////////////////////////////////////////////////////////////
float Stroke::GetOffset() const
{
	return stroke_offset;
}

////////////////////////////////////////////////////////////
float Stroke::GetStartSweep() const
{
	return start_sweep;
}

////////////////////////////////////////////////////////////
float Stroke::GetEndSweep() const
{
	return end_sweep;
}

////////////////////////////////////////////////////////////
Stroke::SweepStyle Stroke::GetStartSweepStyle() const
{
	return start_sweep_style;
}

////////////////////////////////////////////////////////////
Stroke::SweepStyle Stroke::GetEndSweepStyle() const
{
	return end_sweep_style;
}

////////////////////////////////////////////////////////////
float Stroke::GetStippling() const
{
	return stippling;
}

////////////////////////////////////////////////////////////
float Stroke::GetShaking() const
{
	return shaking;
}

////////////////////////////////////////////////////////////
const sf::Color & Stroke::GetStartInnerColor() const
{
	return start_inner_color;
}

////////////////////////////////////////////////////////////
const sf::Color & Stroke::GetStartOuterColor() const
{
	return start_outer_color;
}

////////////////////////////////////////////////////////////
const sf::Color & Stroke::GetEndInnerColor() const
{
	return end_inner_color;
}

////////////////////////////////////////////////////////////
const sf::Color & Stroke::GetEndOuterColor() const
{
	return end_outer_color;
}

////////////////////////////////////////////////////////////
const sf::Texture & Stroke::GetTexture() const
{
	return texture;
}

////////////////////////////////////////////////////////////
const sf::IntRect & Stroke::GetSubRect() const
{
	return sub_rect;
}

////////////////////////////////////////////////////////////
bool Stroke::IsSymetric() const
{
	return symetric;
}

////////////////////////////////////////////////////////////
float Stroke::GetTextureOffset() const
{
	return texture_offset;
}

////////////////////////////////////////////////////////////
float Stroke::GetTexturePan() const
{
	return texture_pan;
}

////////////////////////////////////////////////////////////
float Stroke::GetLength()
{
	if(!is_length_compiled)
		CompileLength();
	return length;
}

////////////////////////////////////////////////////////////
const sf::FloatRect & Stroke::GetBoundingBox()
{
	if(!is_bounding_box_compiled)
		CompileBoundingBox();
	return bounding_box;
}

////////////////////////////////////////////////////////////
bool Stroke::IsDrawable() const
{
	if(joints.size() < 2 || thickness == 0.f || stippling == 1.f)
		return false;
	return true;
}

// Modificateurs de l'objet
////////////////////////////////////////////////////////////
void Stroke::SetJointLinearSpeed(float x, float y)
{
	SetJointLinearSpeed(sf::Vector2f(x, y));
}

////////////////////////////////////////////////////////////
void Stroke::SetJointLinearSpeed(const sf::Vector2f & joint_linear_speed)
{
	this->joint_linear_speed = joint_linear_speed;
}

////////////////////////////////////////////////////////////
void Stroke::SetMaxJoints(unsigned int max_joints)
{
	this->max_joints = max_joints;
}

////////////////////////////////////////////////////////////
void Stroke::SetMaxLength(float max_length)
{
	this->max_length = max_length;
}

////////////////////////////////////////////////////////////
void Stroke::SetMinSegmentLength(float min_segment_length)
{
	this->min_segment_length = min_segment_length;
}

////////////////////////////////////////////////////////////
void Stroke::SetJointLifetime(float joint_lifetime)
{
	this->joint_lifetime = joint_lifetime;
}

////////////////////////////////////////////////////////////
void Stroke::SetThickness(float thickness)
{
	this->thickness = thickness;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetRandomThickness(float min, float max)
{
	random_thickness.x = min;
	random_thickness.y = max;
}

////////////////////////////////////////////////////////////
void Stroke::SetStartThickness(float start_thickness)
{
	this->start_thickness = ToRange(start_thickness);
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetEndThickness(float end_thickness)
{
	this->end_thickness = ToRange(end_thickness);
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetOffset(float stroke_offset)
{
	this->stroke_offset = ToRange(stroke_offset, -1.f);
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetSweep(float sweep)
{
	SetStartSweep(sweep);
	SetEndSweep(sweep);
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetStartSweep(float start_sweep)
{
	this->start_sweep = ToRange(start_sweep);
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetEndSweep(float end_sweep)
{
	this->end_sweep = ToRange(end_sweep);
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetSweepStyle(Stroke::SweepStyle sweep_style)
{
	this->start_sweep_style = sweep_style;
	this->end_sweep_style = sweep_style;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetStartSweepStyle(Stroke::SweepStyle start_sweep_style)
{
	this->start_sweep_style = start_sweep_style;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetEndSweepStyle(Stroke::SweepStyle end_sweep_style)
{
	this->end_sweep_style = end_sweep_style;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetStippling(float stippling)
{
	this->stippling = ToRange(stippling);
}

////////////////////////////////////////////////////////////
void Stroke::SetShaking(float shaking)
{
	this->shaking = shaking;
}

////////////////////////////////////////////////////////////
void Stroke::SetLoop(bool loop)
{
	this->loop = loop;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetDrawMode(Stroke::DrawMode draw_mode)
{
	this->draw_mode = draw_mode;
}

////////////////////////////////////////////////////////////
void Stroke::SetInnerColor(const sf::Color & inner_color)
{
	this->start_inner_color = inner_color;
	this->end_inner_color = inner_color;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetOuterColor(const sf::Color & outer_color)
{
	this->start_outer_color = outer_color;
	this->end_outer_color = outer_color;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetStartInnerColor(const sf::Color & start_inner_color)
{
	this->start_inner_color = start_inner_color;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetStartOuterColor(const sf::Color & start_outer_color)
{
	this->start_outer_color = start_outer_color;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetEndInnerColor(const sf::Color & end_inner_color)
{
	this->end_inner_color = end_inner_color;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetEndOuterColor(const sf::Color & end_outer_color)
{
	this->end_outer_color = end_outer_color;
	is_compiled = false;
}

////////////////////////////////////////////////////////////
void Stroke::SetTexture(const sf::Texture & texture, bool adjust_to_new_size)
{
	if(adjust_to_new_size && (texture.getSize().x > 0) && (texture.getSize().y > 0))
		SetSubRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));

	this->texture = texture;
	draw_mode = Textured;
}

////////////////////////////////////////////////////////////
void Stroke::SetSubRect(const sf::IntRect & sub_rect)
{
	this->sub_rect = sub_rect;
}

////////////////////////////////////////////////////////////
void Stroke::SetSymetric(bool symetric)
{
	this->symetric = symetric;
}

////////////////////////////////////////////////////////////
void Stroke::SetTextureOffset(float texture_offset)
{
	this->texture_offset = texture_offset;
}

////////////////////////////////////////////////////////////
void Stroke::MoveTexture(float texture_offset)
{
	this->texture_offset += texture_offset;
}

////////////////////////////////////////////////////////////
void Stroke::SetTexturePan(float texture_pan)
{
	if(texture_pan < 1)
		this->texture_pan = 1;
	this->texture_pan = texture_pan;
}

////////////////////////////////////////////////////////////
bool Stroke::AddJoint(float x, float y, bool precompile)
{
	return AddJoint(sf::Vector2f(x, y), precompile);
}

////////////////////////////////////////////////////////////
bool Stroke::AddJoint(const sf::Vector2f & position, bool precompile)
{
	Joint joint;
	joint.position = position;
	joint.birth_time = time;
	joint.thickness_scale = myRand(random_thickness.x, random_thickness.y);

	if(joints.empty())
	{
		joints.push_back(joint);
		is_length_compiled = false;
		return true;
	}

	bool joint_added = false;

	// Adding joints
	if(min_segment_length)
	{
			float angle = Angle(joints[joints.size()-1].position, position);
			sf::Vector2f last_point_position = joints[joints.size()-1].position;
			int i = 0;
			while(Distance(joints[joints.size()-1].position, position) >= min_segment_length)
			{
				joint.position = last_point_position + ToVector(angle, (i + 1) * min_segment_length);
				joint.thickness_scale = myRand(random_thickness.x, random_thickness.y);
				joints.push_back(joint);
				is_compiled = false;
				is_length_compiled = false;
				joint_added = true;
				++i;
			}
	}
	else
	{
		if(joints[joints.size()-1].position != position)
		{
			joints.push_back(joint);
			is_compiled = false;
			is_length_compiled = false;
			joint_added = true;
		}
	}


	if(precompile && joints.size() >= 3)
		Compile();

	return joint_added;
}

////////////////////////////////////////////////////////////
bool Stroke::AddJoint(float x, float y, float thickness_scale, bool precompile)
{
	return AddJoint(sf::Vector2f(x, y), thickness_scale, precompile);
}

////////////////////////////////////////////////////////////
bool Stroke::AddJoint(const sf::Vector2f & position, float thickness_scale, bool precompile)
{
	Joint joint;
	joint.position = position;
	joint.birth_time = time;
	joint.thickness_scale = thickness_scale;

	if(joints.empty())
	{
		joints.push_back(joint);
		is_length_compiled = false;
		return true;
	}

	bool joint_added = false;

	// Adding joints
	if(min_segment_length)
	{
			float angle = Angle(joints[joints.size()-1].position, position);
			sf::Vector2f last_point_position = joints[joints.size()-1].position;
			int i = 0;
			while(Distance(joints[joints.size()-1].position, position) >= min_segment_length)
			{
				joint.position = last_point_position + ToVector(angle, (i + 1) * min_segment_length);
				joints.push_back(joint);
				is_compiled = false;
				is_length_compiled = false;
				joint_added = true;
				++i;
			}
	}
	else
	{
		if(joints[joints.size()-1].position != position)
		{
			joints.push_back(joint);
			is_compiled = false;
			is_length_compiled = false;
			joint_added = true;
		}
	}


	if(precompile)
		Compile();

	return joint_added;
}

////////////////////////////////////////////////////////////
void Stroke::Clear()
{
	joints.clear();
}

////////////////////////////////////////////////////////////
void Stroke::Calculate(float time_interval)
{
	// Deleting unwanted points
	if(!joints.empty())
		/* Removing if
		- max joint counter is reached
		- max length is reached
		- points life is passed*/
		while((joints.size() > max_joints && max_joints) || (GetLength() > max_length && max_length) || (time - joints[0].birth_time > joint_lifetime && joint_lifetime > 0))
		{
			joints.pop_front();
			is_compiled = false;
			is_length_compiled = false;
			if(joints.empty())
				break;
		}

	// Moving lefting points
		if(joint_linear_speed != sf::Vector2f(0.f, 0.f))
	{
		for(unsigned int i = 0; i < joints.size(); i++)
		{
			joints[i].position += joint_linear_speed * time_interval;
			joints[i].upper_point += joint_linear_speed * time_interval;
			joints[i].lower_point += joint_linear_speed * time_interval;
		}
	}

	time += time_interval;
}

////////////////////////////////////////////////////////////
void Stroke::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(!is_compiled)
		if(!const_cast<Stroke*>(this)->Compile())
			return;

	if(shaking)
		const_cast<Stroke*>(this)->ComputeOffset();

	switch(draw_mode)
	{
		case Solid:
			DrawSolid(target, states);
			return;
		case Textured:
			DrawTexture(target, states);
			return;
		case Monochrome:
			DrawMonochrome(target, states);
			return;
		case WireFrame:
			DrawWireframe(target, states);
			return;
		default:
			DrawMonochrome(target, states);
			return;
	}
}

////////////////////////////////////////////////////////////
void Stroke::DrawSolid(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = nullptr;

	for(unsigned int i = 1; i < joints.size(); i++)
		DrawSolidBloc(target, states, i, i-1);

	if(loop)
		DrawSolidBloc(target, states, 0, joints.size()-1);
}

////////////////////////////////////////////////////////////
void Stroke::DrawSolidBloc(sf::RenderTarget& target, sf::RenderStates states, unsigned int current_joint, unsigned int previous_joint) const
{
	sf::Vector2f center_previous = ((stippling)? joints[previous_joint].position * (1.f - stippling/2.f) + joints[current_joint].position * (stippling/2.f) : joints[previous_joint].position) + joints[previous_joint].offset;
	sf::Vector2f center_next = ((stippling)? joints[current_joint].position * (1.f - stippling/2.f) + joints[previous_joint].position * (stippling/2.f) : joints[current_joint].position) + joints[current_joint].offset;
	sf::Vector2f upper_previous = ((stippling)? joints[previous_joint].upper_point * (1.f - stippling/2.f) + joints[current_joint].upper_point * (stippling/2.f) : joints[previous_joint].upper_point) + joints[previous_joint].offset;
	sf::Vector2f upper_next = ((stippling)? joints[current_joint].upper_point * (1.f - stippling/2.f) + joints[previous_joint].upper_point * (stippling/2.f) : joints[current_joint].upper_point) + joints[current_joint].offset;
	sf::Vector2f lower_previous = ((stippling)? joints[previous_joint].lower_point * (1.f - stippling/2.f) + joints[current_joint].lower_point * (stippling/2.f) : joints[previous_joint].lower_point) + joints[previous_joint].offset;
	sf::Vector2f lower_next = ((stippling)? joints[current_joint].lower_point * (1.f - stippling/2.f) + joints[previous_joint].lower_point * (stippling/2.f) : joints[current_joint].lower_point) + joints[current_joint].offset;

	// First Side
	sf::VertexArray first(sf::TrianglesStrip, 4);
	first[0].position = center_previous;
	first[0].color = joints[previous_joint].inner_color;
	first[1].position = center_next;
	first[1].color = joints[current_joint].inner_color;
	first[2].position = upper_previous;
	first[2].color = joints[previous_joint].outer_color;
	first[3].position = upper_next;
	first[3].color = joints[current_joint].outer_color;

	// Second Side
	sf::VertexArray second(sf::TrianglesStrip, 4);
	second[0].position = center_previous;
	second[0].color = joints[previous_joint].inner_color;
	second[1].position = center_next;
	second[1].color = joints[current_joint].inner_color;
	second[2].position = lower_previous;
	second[2].color = joints[previous_joint].outer_color;
	second[3].position = lower_next;
	second[3].color = joints[current_joint].outer_color;

	target.draw(first, states);
	target.draw(second, states);
}

////////////////////////////////////////////////////////////
void Stroke::DrawTexture(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(texture.getSize().x > 0)
	{
		const sf::FloatRect coords(sub_rect.left / texture.getSize().x, sub_rect.top / texture.getSize().y,
									sub_rect.width / texture.getSize().x, sub_rect.height / texture.getSize().y);
		states.texture = &texture;

		// We repeat the texture to ensure no gap will appear between two blocs
		if(texture.isSmooth())
		{
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		}

		float offset = std::fmod(texture_offset, 1.f);
		if(offset < 0.f)
			offset += 1.f;
		float previous_accumulator = offset;
		float offset_accumulator = offset;

        //bool is_integer = (texture_pan == (int)texture_pan);

		for(unsigned int i = 1; i < joints.size(); i++)
		{
			sf::Vector2f center_previous = ((stippling)? joints[i-1].position * (1.f - stippling/2.f) + joints[i].position * (stippling/2.f) : joints[i-1].position) + joints[i-1].offset;
			sf::Vector2f center_next = ((stippling)? joints[i].position * (1.f - stippling/2.f) + joints[i-1].position * (stippling/2.f) : joints[i].position) + joints[i].offset;
			sf::Vector2f upper_previous = ((stippling)? joints[i-1].upper_point * (1.f - stippling/2.f) + joints[i].upper_point * (stippling/2.f) : joints[i-1].upper_point) + joints[i-1].offset;
			sf::Vector2f upper_next = ((stippling)? joints[i].upper_point * (1.f - stippling/2.f) + joints[i-1].upper_point * (stippling/2.f) : joints[i].upper_point) + joints[i].offset;
			sf::Vector2f lower_previous = ((stippling)? joints[i-1].lower_point * (1.f - stippling/2.f) + joints[i].lower_point * (stippling/2.f) : joints[i-1].lower_point) + joints[i-1].offset;
			sf::Vector2f lower_next = ((stippling)? joints[i].lower_point * (1.f - stippling/2.f) + joints[i-1].lower_point * (stippling/2.f) : joints[i].lower_point) + joints[i].offset;

			previous_accumulator = offset_accumulator;
			offset_accumulator += 1/texture_pan;
			float alpha = (1.f - previous_accumulator) / (offset_accumulator - previous_accumulator);

			// Testing if the bloc is split by the texture
			if(offset_accumulator > 1.f)
			{
				sf::Vector2f lower_separator = lower_next * alpha + lower_previous * (1.f - alpha);
				sf::Vector2f center_separator = center_next * alpha + center_previous * (1.f - alpha);
				sf::Vector2f upper_separator = upper_next * alpha + upper_previous * (1.f - alpha);
				sf::FloatRect left_bloc_coords = coords;
				left_bloc_coords.width = coords.width / texture_pan * alpha;
				left_bloc_coords.left += coords.width * previous_accumulator;

				sf::FloatRect right_bloc_coords = coords;
				right_bloc_coords.width = coords.width / texture_pan * (1.f - alpha);

				// Splitting
				// First part
				DrawTexturedBloc(target, states, upper_previous, upper_separator, center_previous, center_separator, lower_previous, lower_separator, left_bloc_coords);


				// Second part
				DrawTexturedBloc(target, states, upper_separator, upper_next, center_separator, center_next, lower_separator, lower_next, right_bloc_coords);
				offset_accumulator -= 1.f;
			}
			else
			{
				sf::FloatRect bloc_coords = coords;
				bloc_coords.width = coords.width / texture_pan;

				if(previous_accumulator >= 1.f)
					bloc_coords.left += coords.width * (previous_accumulator - 1.f);
				else
					bloc_coords.left += coords.width * previous_accumulator;

				// Normal rendering
				DrawTexturedBloc(target, states, upper_previous, upper_next, center_previous, center_next, lower_previous, lower_next, bloc_coords);
			}
		}

		if(loop)
		{
			unsigned int current_joint = 0;
			unsigned int previous_joint = joints.size()-1;
			sf::Vector2f center_previous = ((stippling)? joints[previous_joint].position * (1.f - stippling/2.f) + joints[current_joint].position * (stippling/2.f) : joints[previous_joint].position) + joints[previous_joint].offset;
			sf::Vector2f center_next = ((stippling)? joints[current_joint].position * (1.f - stippling/2.f) + joints[previous_joint].position * (stippling/2.f) : joints[current_joint].position) + joints[current_joint].offset;
			sf::Vector2f upper_previous = ((stippling)? joints[previous_joint].upper_point * (1.f - stippling/2.f) + joints[current_joint].upper_point * (stippling/2.f) : joints[previous_joint].upper_point) + joints[previous_joint].offset;
			sf::Vector2f upper_next = ((stippling)? joints[current_joint].upper_point * (1.f - stippling/2.f) + joints[previous_joint].upper_point * (stippling/2.f) : joints[current_joint].upper_point) + joints[current_joint].offset;
			sf::Vector2f lower_previous = ((stippling)? joints[previous_joint].lower_point * (1.f - stippling/2.f) + joints[current_joint].lower_point * (stippling/2.f) : joints[previous_joint].lower_point) + joints[previous_joint].offset;
			sf::Vector2f lower_next = ((stippling)? joints[current_joint].lower_point * (1.f - stippling/2.f) + joints[previous_joint].lower_point * (stippling/2.f) : joints[current_joint].lower_point) + joints[current_joint].offset;

			previous_accumulator = offset_accumulator;
			offset_accumulator += 1/texture_pan;
			float alpha = (1.f - previous_accumulator) / (offset_accumulator - previous_accumulator);

			// Testing if the bloc is split by the texture
			if(offset_accumulator > 1.f)
			{
				sf::Vector2f lower_separator = lower_next * alpha + lower_previous * (1.f - alpha);
				sf::Vector2f center_separator = center_next * alpha + center_previous * (1.f - alpha);
				sf::Vector2f upper_separator = upper_next * alpha + upper_previous * (1.f - alpha);
				sf::FloatRect left_bloc_coords = coords;
				left_bloc_coords.width = coords.width / texture_pan * alpha;
				left_bloc_coords.left += coords.width * previous_accumulator;

				sf::FloatRect right_bloc_coords = coords;
				right_bloc_coords.width = coords.width / texture_pan * (1.f - alpha);

				// Splitting
				// First part
				DrawTexturedBloc(target, states, upper_previous, upper_separator, center_previous, center_separator, lower_previous, lower_separator, left_bloc_coords);


				// Second part
				DrawTexturedBloc(target, states, upper_separator, upper_next, center_separator, center_next, lower_separator, lower_next, right_bloc_coords);
				offset_accumulator -= 1.f;
			}
			else
			{
				sf::FloatRect bloc_coords = coords;
				bloc_coords.width = coords.width / texture_pan;

				if(previous_accumulator >= 1.f)
					bloc_coords.left += coords.width * (previous_accumulator - 1.f);
				else
					bloc_coords.left += coords.width * previous_accumulator;

				// Normal rendering
				DrawTexturedBloc(target, states, upper_previous, upper_next, center_previous, center_next, lower_previous, lower_next, bloc_coords);
			}
		}
	}
	else
	{
		DrawMonochrome(target, states);
	}
}
////////////////////////////////////////////////////////////
void Stroke::DrawTexturedBloc(sf::RenderTarget & target, sf::RenderStates states, const sf::Vector2f & TopLeft, const sf::Vector2f & TopRight, const sf::Vector2f & CenterLeft, const sf::Vector2f & CenterRight, const sf::Vector2f & BottomLeft, const sf::Vector2f & BottomRight, const sf::FloatRect & TexCoords) const
{
	if(symetric)
	{
		sf::VertexArray first(sf::TrianglesStrip, 4);
		first[0].position = CenterLeft;
		first[0].texCoords = sf::Vector2f(TexCoords.left, TexCoords.height + TexCoords.top);
		first[1].position = CenterRight;
		first[1].texCoords = sf::Vector2f(TexCoords.width + TexCoords.left, TexCoords.height + TexCoords.top);
		first[2].position = TopLeft;
		first[2].texCoords = sf::Vector2f(TexCoords.left, TexCoords.top);
		first[3].position = TopRight;
		first[3].texCoords = sf::Vector2f(TexCoords.width + TexCoords.left, TexCoords.top);

		sf::VertexArray second(sf::TrianglesStrip, 4);
		second[0].position = CenterLeft;
		second[0].texCoords = sf::Vector2f(TexCoords.left, TexCoords.height + TexCoords.top);
		second[1].position = CenterRight;
		second[1].texCoords = sf::Vector2f(TexCoords.width + TexCoords.left, TexCoords.height + TexCoords.top);
		second[2].position = BottomLeft;
		second[2].texCoords = sf::Vector2f(TexCoords.left, TexCoords.top);
		second[3].position = BottomRight;
		second[3].texCoords = sf::Vector2f(TexCoords.width + TexCoords.left, TexCoords.top);

		target.draw(first, states);
		target.draw(second, states);
	}
	else
	{
		sf::VertexArray first(sf::TrianglesStrip, 4);
		first[0].position = TopLeft;
		first[0].texCoords = sf::Vector2f(TexCoords.left, TexCoords.height + TexCoords.top);
		first[1].position = TopRight;
		first[1].texCoords = sf::Vector2f(TexCoords.width + TexCoords.left, TexCoords.height + TexCoords.top);
		first[2].position = BottomLeft;
		first[2].texCoords = sf::Vector2f(TexCoords.left, TexCoords.top);
		first[3].position = BottomRight;
		first[3].texCoords = sf::Vector2f(TexCoords.width + TexCoords.left, TexCoords.top);

		target.draw(first, states);
	}
}
////////////////////////////////////////////////////////////
void Stroke::DrawMonochrome(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = nullptr;

	for(unsigned int i = 1; i < joints.size(); i++)
		DrawMonochromeBloc(target, states, i, i-1);

	if(loop)
		DrawMonochromeBloc(target, states, 0, joints.size()-1);
}

////////////////////////////////////////////////////////////
void Stroke::DrawMonochromeBloc(sf::RenderTarget & target, sf::RenderStates states, unsigned int current_joint, unsigned int previous_joint) const
{
	sf::Vector2f center_previous = ((stippling)? joints[previous_joint].position * (1.f - stippling/2.f) + joints[current_joint].position * (stippling/2.f) : joints[previous_joint].position) + joints[previous_joint].offset;
	sf::Vector2f center_next = ((stippling)? joints[current_joint].position * (1.f - stippling/2.f) + joints[previous_joint].position * (stippling/2.f) : joints[current_joint].position) + joints[current_joint].offset;
	sf::Vector2f upper_previous = ((stippling)? joints[previous_joint].upper_point * (1.f - stippling/2.f) + joints[current_joint].upper_point * (stippling/2.f) : joints[previous_joint].upper_point) + joints[previous_joint].offset;
	sf::Vector2f upper_next = ((stippling)? joints[current_joint].upper_point * (1.f - stippling/2.f) + joints[previous_joint].upper_point * (stippling/2.f) : joints[current_joint].upper_point) + joints[current_joint].offset;
	sf::Vector2f lower_previous = ((stippling)? joints[previous_joint].lower_point * (1.f - stippling/2.f) + joints[current_joint].lower_point * (stippling/2.f) : joints[previous_joint].lower_point) + joints[previous_joint].offset;
	sf::Vector2f lower_next = ((stippling)? joints[current_joint].lower_point * (1.f - stippling/2.f) + joints[previous_joint].lower_point * (stippling/2.f) : joints[current_joint].lower_point) + joints[current_joint].offset;

	// First Side
	sf::VertexArray first(sf::TrianglesStrip, 4);
	first[0].position = center_previous;
	first[1].position = center_next;
	first[2].position = upper_previous;
	first[3].position = upper_next;

	// Second Side
	sf::VertexArray second(sf::TrianglesStrip, 4);
	first[0].position = center_previous;
	first[1].position = center_next;
	first[2].position = lower_previous;
	first[3].position = lower_next;

	target.draw(first, states);
	target.draw(second, states);
}

////////////////////////////////////////////////////////////
void Stroke::DrawWireframe(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = nullptr;

	std::size_t size = joints.size();
	if(loop)
		++size;

	sf::VertexArray first(sf::LinesStrip, size);
	for(unsigned int i = 0; i < joints.size(); i++)
		first[i].position = sf::Vector2f(joints[i].position.x + joints[i].offset.x, joints[i].position.y + joints[i].offset.y);

	if(loop)
		first[size-1].position = first[0].position;

	sf::VertexArray second(sf::LinesStrip, size);
	for(unsigned int i = 0; i < joints.size(); i++)
	{
		second[i].position = sf::Vector2f(joints[i].upper_point.x + joints[i].offset.x, joints[i].upper_point.y + joints[i].offset.y);
		second[i].color = sf::Color(128, 128, 128);
	}

	if(loop)
	{
		second[size-1].position = second[0].position;
		second[size-1].color = second[0].color;
	}

	sf::VertexArray third(sf::LinesStrip, size);
	for(unsigned int i = 0; i < joints.size(); i++)
	{
		third[i].position = sf::Vector2f(joints[i].lower_point.x + joints[i].offset.x, joints[i].lower_point.y + joints[i].offset.y);
		third[i].color = sf::Color(128, 128, 128);
	}

	if(loop)
	{
		third[size-1].position = third[0].position;
		third[size-1].color = third[0].color;
	}

	sf::VertexArray forth(sf::Lines, 2*joints.size());
	for(unsigned int i = 0; i < joints.size(); i++)
	{
		forth[i].position = sf::Vector2f(joints[i].upper_point.x + joints[i].offset.x, joints[i].upper_point.y + joints[i].offset.y);
		forth[i].color = sf::Color(128, 128, 128);
		forth[i+1].position = sf::Vector2f(joints[i].lower_point.x + joints[i].offset.x, joints[i].lower_point.y + joints[i].offset.y);
		forth[i+1].color = sf::Color(128, 128, 128);
	}

	const_cast<Stroke*>(this)->GetBoundingBox();
	sf::VertexArray fifth(sf::LinesStrip, 5);
	fifth[0].position = sf::Vector2f(bounding_box.left, bounding_box.top);
	fifth[0].color = sf::Color(32, 164, 32);
	fifth[1].position = sf::Vector2f(bounding_box.left, bounding_box.top + bounding_box.height);
	fifth[1].color = sf::Color(32, 164, 32);
	fifth[2].position = sf::Vector2f(bounding_box.top + bounding_box.height, bounding_box.top + bounding_box.height);
	fifth[2].color = sf::Color(32, 164, 32);
	fifth[3].position = sf::Vector2f(bounding_box.left + bounding_box.width, bounding_box.top);
	fifth[3].color = sf::Color(32, 164, 32);
	fifth[4].position = fifth[0].position;
	fifth[4].color = fifth[0].color;

	target.draw(first, states);
	target.draw(second, states);
	target.draw(third, states);
	target.draw(forth, states);
	target.draw(fifth, states);
}

////////////////////////////////////////////////////////////
bool Stroke::Compile()
{
	if(joints.size() < 2)
		return false;

	float total_length = GetLength();

	// Determining joints' angles
	if(loop)
	{
		{
			// First joint
			joints[0].angle = Angle(joints[0].position, joints[1].position);

			// Calculating in and out angles
			float angle1 = Angle(joints[joints.size()-1].position, joints[0].position);
			float angle2 = Angle(joints[0].position, joints[1].position);

			// Calculating joint angle
			joints[0].angle = (angle1 + angle2)/2.f;

			// In certain cases, previous line give the opposite of the wanted angle, applying correction if so
			if(Sign(angle1) != Sign(angle2) && ((angle1 > 90 || angle1 < -90) || (angle2 > 90 || angle2 < -90)) && (Norm(angle1) + Norm(angle2) > 180))
			{
				if(Sign(joints[0].angle) == 1)
					joints[0].angle -= 180;
				else
					joints[0].angle += 180;
			}
		}

		{
			// Last joint
			joints[joints.size()-1].angle = Angle(joints[joints.size()-2].position, joints[joints.size()-1].position);

			// Calculating in and out angles
			float angle1 = Angle(joints[joints.size()-2].position, joints[joints.size()-1].position);
			float angle2 = Angle(joints[joints.size()-1].position, joints[0].position);

			// Calculating joint angle
			joints[joints.size()-1].angle = (angle1 + angle2)/2.f;

			// In certain cases, previous line give the opposite of the wanted angle, applying correction if so
			if(Sign(angle1) != Sign(angle2) && ((angle1 > 90 || angle1 < -90) || (angle2 > 90 || angle2 < -90)) && (Norm(angle1) + Norm(angle2) > 180))
			{
				if(Sign(joints[joints.size()-1].angle) == 1)
					joints[joints.size()-1].angle -= 180;
				else
					joints[joints.size()-1].angle += 180;
			}
		}
	}
	else
	{
		// First joint
		joints[0].angle = Angle(joints[0].position, joints[1].position);

		// Last joint
		joints[joints.size()-1].angle = Angle(joints[joints.size()-2].position, joints[joints.size()-1].position);
	}

	// other joints
	for(unsigned int i = 1; i < joints.size()-1; i++)
	{
		// Calculating in and out angles
		float angle1 = Angle(joints[i-1].position, joints[i].position);
		float angle2 = Angle(joints[i].position, joints[i+1].position);

		// Calculating joint angle
		joints[i].angle = (angle1 + angle2)/2.f;

		// In certain cases, previous line give the opposite of the wanted angle, applying correction if so
		if(Sign(angle1) != Sign(angle2) && ((angle1 > 90 || angle1 < -90) || (angle2 > 90 || angle2 < -90)) && (Norm(angle1) + Norm(angle2) > 180))
		{
			if(Sign(joints[i].angle) == 1)
				joints[i].angle -= 180;
			else
				joints[i].angle += 180;
		}
	}


	// Compiling outer points, inner points, thickness and color of each joint
	float l = 0;
	for(unsigned int i = 0; i < joints.size(); i++)
	{
		if(i > 0)
			l += Distance(joints[i-1].position, joints[i].position);

		float alpha = l / total_length;
		float off = (stroke_offset + 1.f)/2.f;
		float t = (thickness/2.f) * joints[i].thickness_scale;

		joints[i].thickness = ComputeThickness(alpha, off, t);
		joints[i].upper_point = joints[i].position + ToVector(joints[i].angle + 90, joints[i].thickness);
		joints[i].lower_point = joints[i].position + ToVector(joints[i].angle - 90, joints[i].thickness);
		if(draw_mode == Solid || draw_mode == Textured)
		{
			joints[i].inner_color = ColorInterpolation(start_inner_color, end_inner_color, alpha);
			joints[i].outer_color = ColorInterpolation(start_outer_color, end_outer_color, alpha);
		}
	}

	is_bounding_box_compiled = false;
	is_compiled = true;
	return true;
}

////////////////////////////////////////////////////////////
void Stroke::ComputeOffset()
{
	for(unsigned int i = 0; i < joints.size(); i++)
	{
		float offset = myRand(-shaking, shaking);
		joints[i].offset =ToVector(joints[i].angle + 90, offset);
	}
}

////////////////////////////////////////////////////////////
void Stroke::CompileBoundingBox()
{
	// Compiling bounding box
	float Left = joints[0].position.x;
	float Right = joints[0].position.x;
	float Top = joints[0].position.y;
	float Bottom = joints[0].position.y;
	for(unsigned int i = 0; i < joints.size(); i++)
	{
		if(joints[i].lower_point.x < Left)
			Left = joints[i].lower_point.x;
		if(joints[i].upper_point.x < Left)
			Left = joints[i].upper_point.x;

		if(joints[i].lower_point.y < Top)
			Top = joints[i].lower_point.y;
		if(joints[i].upper_point.y < Top)
			Top = joints[i].upper_point.y;

		if(joints[i].lower_point.x > Right)
			Right = joints[i].lower_point.x;
		if(joints[i].upper_point.x > Right)
			Right = joints[i].upper_point.x;

		if(joints[i].lower_point.y > Bottom)
			Bottom = joints[i].lower_point.y;
		if(joints[i].upper_point.y > Bottom)
			Bottom = joints[i].upper_point.y;
	}

	bounding_box.left = Left;
	bounding_box.top = Top;
	bounding_box.width = Right - Left;
	bounding_box.height = Bottom - Top;
	is_bounding_box_compiled = true;
}

////////////////////////////////////////////////////////////
void Stroke::CompileLength()
{
	float temp_length = 0;

	if(joints.size() > 1)
		for(unsigned int i = 1; i < joints.size(); i++)
			temp_length += Distance(joints[i-1].position, joints[i].position);
	length = temp_length;
	is_length_compiled = true;
}

////////////////////////////////////////////////////////////
float Stroke::ComputeThickness(float alpha, float offset, float t)
{
	if(alpha <= offset)
	{
		float a = (alpha * (1.f - end_thickness))/offset + end_thickness;
		if(end_sweep)
		{
			float b = ApplySweep(end_sweep_style, a);
			t *= a * (1.f - end_sweep) + b * end_sweep;
		}
		else
			t *= a;
	}
	else
	{
		float a = ((alpha - 1.f) * (1.f - start_thickness))/(offset - 1.f) + start_thickness;
		if(start_sweep)
		{
			float b = ApplySweep(start_sweep_style, a);
			t *= a * (1.f - start_sweep) + b * start_sweep;
		}
		else
			t *= a;
	}
	return t;
}

////////////////////////////////////////////////////////////
float Stroke::ApplySweep(int sweep_style, float a)
{
	switch(sweep_style)
	{
		case Soft:
			return sin(a/2.f * M_PI);
		case Softer:
			return sin(sin(a/2.f * M_PI)/2.f * M_PI);
		case Sharp:
			return sin(a/2.f * M_PI) * sin(a/2.f * M_PI);
		default:
			return a;
	}
}



////////////////////////////////////////////////////////////
inline float Stroke::ToRange(float value, float min, float max)
{
	if(value < min) value = min;
	if(value > max) value = max;
	return value;
}

////////////////////////////////////////////////////////////
inline float Stroke::ToDeg(float angle)
{

	return angle * 180.f / M_PI;
}

////////////////////////////////////////////////////////////
inline float Stroke::ToRad(float angle)
{

	return angle * M_PI / 180.f;
}

////////////////////////////////////////////////////////////
inline float Stroke::Angle(const sf::Vector2f & P1, const sf::Vector2f & P2)
{
	if(P1 == P2)
		return 0;
	else
		return ToDeg(atan2(P1.y - P2.y, P2.x - P1.x));
}

////////////////////////////////////////////////////////////
inline float Stroke::Distance(const sf::Vector2f & P1, const sf::Vector2f & P2)
{
	return sqrt((P1.x - P2.x) * (P1.x - P2.x) + (P1.y - P2.y) * (P1.y - P2.y));
}

////////////////////////////////////////////////////////////
inline sf::Vector2f Stroke::ToVector(float angle, float length)
{
	angle = ToRad(angle);
	return sf::Vector2f(length * cos(angle), length * -sin(angle));
}

////////////////////////////////////////////////////////////
inline float Stroke::Sign(float value)
{
	if(value >= 0)
		return 1;
	else
		return -1;
}

////////////////////////////////////////////////////////////
inline float Stroke::Norm(float value)
{
	return value * Sign(value);
}

////////////////////////////////////////////////////////////
inline sf::Color Stroke::ColorInterpolation(const sf::Color & color1, const sf::Color & color2, float alpha)
{
	return sf::Color(static_cast<sf::Uint8>(static_cast<float>(color1.r) * alpha + static_cast<float>(color2.r) * (1.f - alpha)),
		static_cast<sf::Uint8>(static_cast<float>(color1.g) * alpha + static_cast<float>(color2.g) * (1.f - alpha)),
		static_cast<sf::Uint8>(static_cast<float>(color1.b) * alpha + static_cast<float>(color2.b) * (1.f - alpha)),
		static_cast<sf::Uint8>(static_cast<float>(color1.a) * alpha + static_cast<float>(color2.a) * (1.f - alpha)));

}
