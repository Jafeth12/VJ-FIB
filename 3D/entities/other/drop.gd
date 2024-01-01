# class_name Drop extends GenericEntity
extends RigidBody3D

@export var drop_type : Enums.DROP_TYPE = Enums.DROP_TYPE.HEALTH
@onready var player = get_node("/root/main/Player")

var drop_amount = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	load_texture()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
	
#------------------------------------
func load_texture() -> void:
	match drop_type:
		Enums.DROP_TYPE.HEALTH:
			$icon.texture = load("res://art/ui/drops/HealthDrop.png")
		Enums.DROP_TYPE.AMMO:
			$icon.texture = load("res://art/ui/drops/AmmoDrop_Bullets.png")
		Enums.DROP_TYPE.RIFLE:
			$icon.texture = load("res://art/ui/hud/rifle_ui.png")

func set_drop_type(type: Enums.DROP_TYPE):
	drop_type = type
	load_texture()

func set_drop_amount(amount):
	drop_amount = amount

# func set_alpha(alpha):
# 	entity_alpha = alpha

func _on_body_entered(body):
	if body is Player:
		match drop_type:
			Enums.DROP_TYPE.HEALTH:
				player.player_give_health(drop_amount)
			Enums.DROP_TYPE.AMMO:
				player.player_give_ammo(drop_amount)
			Enums.DROP_TYPE.RIFLE:
				pass

		queue_free()
	
