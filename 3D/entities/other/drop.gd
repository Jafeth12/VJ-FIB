# class_name Drop extends GenericEntity
extends Node3D

enum DROP_TYPE {HEALTH, AMMO, RIFLE}

@export var drop_type : DROP_TYPE = DROP_TYPE.HEALTH
@onready var player = get_node("/root/main/Player")

var drop_amount = 0

var sprites = [
	Image.load_from_file("res://art/ui/drops/HealthDrop.png"),
	Image.load_from_file("res://art/ui/drops/AmmoDrop_Bullets.png"),
	Image.load_from_file("res://art/ui/hud/rifle_ui.png"),
]

# Called when the node enters the scene tree for the first time.
func _ready():
	$icon.texture = ImageTexture.create_from_image(sprites[(int)(drop_type)])

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	#print("drop alpha: ", entity_alpha)
	print("drop pos: ", get_position())

# 0 = health, 1 = ammo, 2 = rifle
func set_drop_type(type):
	drop_type = type
	$icon.texture = ImageTexture.create_from_image(sprites[(int)(drop_type)])

func set_drop_amount(amount):
	drop_amount = amount

# func set_alpha(alpha):
# 	entity_alpha = alpha

func _on_body_entered(body):
	if body is Player:
		match drop_type:
			DROP_TYPE.HEALTH:
				player.player_give_health(drop_amount)
			DROP_TYPE.AMMO:
				player.player_give_ammo(drop_amount)
			DROP_TYPE.RIFLE:
				pass

		queue_free()
	
