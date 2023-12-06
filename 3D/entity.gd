extends CharacterBody3D


@export var SPEED = PI/8 # 1 lap = 16 secs.
const JUMP_VELOCITY = 10

# Get the gravity from the project settings to be synced with RigidBody nodes.
# var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")
const gravity = 10 # TODO: abstraer esto
const player_radius = 18 # TODO: abtraer esto
@export var alpha = 0


func _physics_process(delta):
	# Add the gravity.
	if not is_on_floor():
		velocity.y -= gravity * delta

	# Handle jump.
	if Input.is_action_just_pressed("jump") and is_on_floor():
		velocity.y = JUMP_VELOCITY

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var right_pressed = Input.is_action_pressed("move_right")
	var left_pressed = Input.is_action_pressed("move_left")

	# Get direction of movement
	var circular_dir = 0;
	if right_pressed && !left_pressed:
		circular_dir = 1
	elif left_pressed && !right_pressed:
		circular_dir = -1

	# Update alpha
	alpha += circular_dir * SPEED * delta
	var next_xz = get_next_xz()
	velocity.x = (next_xz.x - get_position().x)/delta
	velocity.z = (next_xz.y - get_position().z)/delta
	if move_and_slide():
		set_alpha_by_position(get_position())

func _process(delta):
	look_at(Vector3(0, get_position().y, 0))

func get_next_xz():
	return Vector2(player_radius*sin(alpha), player_radius*cos(alpha))

func set_alpha_by_position(pos):
	alpha = asin(pos.x/player_radius)
	# alpha = acos(pos.z/player_radius) # same shit
