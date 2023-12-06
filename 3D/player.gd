extends CharacterBody3D

enum ANIMATION_STATES { IDLE, WALK, JUMP, CROUCH }
enum FACING { LEFT, RIGHT }

@export var SPEED = PI/8 # 1 lap = 16 secs.
const JUMP_VELOCITY = 7

# Get the gravity from the project settings to be synced with RigidBody nodes.
# var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")
const gravity = 15 # TODO: abstraer esto
const player_radius = 18 # TODO: abtraer esto
@export var alpha = 0

@export var our_scale = 3

var anim_state = ANIMATION_STATES.IDLE
var facing = FACING.RIGHT

func _ready():
	$sprite.set_scale(Vector3(our_scale, our_scale, our_scale))
	$sprite.play("idle")

func _process(delta):
	update_facing()
	update_anim_state()
	look_at(Vector3(0, get_position().y, 0))

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
		facing = FACING.RIGHT
	elif left_pressed && !right_pressed:
		circular_dir = -1
		facing = FACING.LEFT

	# Update alpha
	if !is_crouching():
		alpha += circular_dir * SPEED * delta
	var next_xz = get_next_xz()
	velocity.x = (next_xz.x - get_position().x)/delta
	velocity.z = (next_xz.y - get_position().z)/delta

	var length = velocity.length()
	if length < 0.005:
		velocity = Vector3(0, 0, 0)

	move_and_slide()
	if is_on_wall():
		set_alpha_by_position(get_position())

func update_anim_state():
	var current_anim = $sprite.animation
	# prox_estado:
	match anim_state:
		ANIMATION_STATES.IDLE:
			if velocity.y != 0:
				anim_state = ANIMATION_STATES.JUMP
			elif Input.is_action_pressed("crouch"):
				anim_state = ANIMATION_STATES.CROUCH
			elif velocity.length() > 0:
				anim_state = ANIMATION_STATES.WALK

		ANIMATION_STATES.WALK:
			if velocity.y != 0:
				anim_state = ANIMATION_STATES.JUMP
			elif Input.is_action_pressed("crouch"):
				anim_state = ANIMATION_STATES.CROUCH
			elif velocity.length() == 0:
				anim_state = ANIMATION_STATES.IDLE

		ANIMATION_STATES.JUMP:
			if is_on_floor():
				if velocity.length() > 0:
					anim_state = ANIMATION_STATES.WALK
				elif velocity.length() == 0:
					anim_state = ANIMATION_STATES.IDLE

		ANIMATION_STATES.CROUCH:
			if velocity.y != 0:
				anim_state = ANIMATION_STATES.JUMP
			elif !Input.is_action_pressed("crouch"):
				if velocity.length() == 0:
					anim_state = ANIMATION_STATES.IDLE
				elif velocity.length() > 0:
					anim_state = ANIMATION_STATES.WALK

	# logica_salida:
	match anim_state:
		ANIMATION_STATES.IDLE:
			if current_anim != "idle":
				$sprite.play("idle")

		ANIMATION_STATES.WALK:
			if current_anim != "walk":
				$sprite.play("walk")

		ANIMATION_STATES.JUMP:
			if current_anim != "jump":
				$sprite.play("jump")

		ANIMATION_STATES.CROUCH:
			if current_anim != "crouch":
				$sprite.play("crouch")

func update_facing():
	if facing == FACING.RIGHT:
		$sprite.set_scale(Vector3(-our_scale, our_scale, our_scale))
	elif facing == FACING.LEFT:
		$sprite.set_scale(Vector3(our_scale, our_scale, our_scale))

func get_next_xz():
	return Vector2(player_radius*sin(alpha), player_radius*cos(alpha))

func set_alpha_by_position(pos):
	alpha = asin(pos.x/player_radius)
	# alpha = acos(pos.z/player_radius) # same shit

func is_crouching():
	return Input.is_action_pressed("crouch") && is_on_floor()
