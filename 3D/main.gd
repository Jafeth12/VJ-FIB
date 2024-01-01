extends Node

@onready var hud = $CanvasLayer/HUD

# Called when the node enters the scene tree for the first time.
func _ready():
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

# -----------------------------

func _on_player_took_damage(new_health: int):
	hud.set_health(new_health)

func _on_player_selected_rifle():
	hud.select_rifle()

func _on_player_selected_pistol():
	hud.select_pistol()

func _on_player_died():
	pass # Replace with function body.

func _on_player_shot_weapon(ammo_pistol: int, ammo_rifle: int):
	hud.set_ammo(ammo_pistol, ammo_rifle)
