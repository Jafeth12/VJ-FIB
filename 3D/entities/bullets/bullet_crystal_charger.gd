extends GenericBullet

func _ready():
	super._ready()

func init(pos: Vector3, alpha: float, direction: EntityDirection, radius: float, playerCrouching: bool):
	pos.y += 0.20
	super.init(pos, alpha, direction, radius, playerCrouching)

func _process(_delta):
	super._process(_delta)

func _physics_process(delta):
	super._physics_process(delta)

# @override
func bullet_body_entered(body: Node3D) -> void:
	if body is Player:
		# print("Bullet hit enemy")
		if body.player_is_dead():
			return
		if body.player_is_dodging():
			return
		body.player_take_damage(BULLET_DAMAGE)

	elif body is StaticBody3D:
		pass
	else:
		return

	queue_free()
