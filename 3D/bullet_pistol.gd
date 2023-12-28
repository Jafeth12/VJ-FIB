class_name BulletPistol extends GenericBullet

func _ready():
	BULLET_SPEED = PI/2.3
	BULLET_MAX_TRAVEL_DISTANCE = PI/2.5
	BULLET_DAMAGE = 25
	super._ready()

func init(pos: Vector3, alpha: float, direction: EntityDirection, radius: float, playerCrouching: bool):
	if playerCrouching:
		pos.y += 0.35
	else:
		pos.y += 0.5

	super.init(pos, alpha, direction, radius, playerCrouching)
		

func _process(_delta):
	super._process(_delta)

func _physics_process(delta):
	super._physics_process(delta)
