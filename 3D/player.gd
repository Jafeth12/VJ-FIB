extends CharacterBody3D


@export var SPEED = PI/8 # 1 lap = 16 secs.
const JUMP_VELOCITY = 10	

# Get the gravity from the project settings to be synced with RigidBody nodes.
# var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")
const gravity = 10 # TODO: abstraer esto
const player_radius = 18 # TODO: abtraer esto
var alpha = 0


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
	var next_position = Vector3(player_radius*sin(alpha), velocity.y*delta+get_position().y, player_radius*cos(alpha))
	velocity.x = (next_position.x - get_position().x)/delta
	velocity.z = (next_position.z - get_position().z)/delta
	move_and_slide()
