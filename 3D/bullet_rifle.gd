class_name BulletRifle extends GenericBullet

func _ready():
	BULLET_SPEED = PI/1.3
	BULLET_MAX_TRAVEL_DISTANCE = 1.7*PI
	BULLET_DAMAGE = 50
	super._ready()

func init(pos: Vector3, alpha: float, direction: EntityDirection, radius: float, playerCrouching: bool):
	if playerCrouching:
		pos.y += 0.4
	else:
		pos.y += 0.55

	super.init(pos, alpha, direction, radius, playerCrouching)

func _process(_delta):
	super._process(_delta)

func _physics_process(delta):
	super._physics_process(delta)

