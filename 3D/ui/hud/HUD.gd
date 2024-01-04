extends Control

# Called when the node enters the scene tree for the first time.
func _ready():
	hide_switch_circle_interaction()
	hide_chest_interaction()
	hide_next_height_interaction()
	hide_next_level_interaction()
	# hide_rifle()
	select_pistol()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

# ------------------------------

func set_health(health: int) -> void:
	$health_bar.value = health
	$health_label.text = str(health)

func set_ammo(ammo_pistol: int, ammo_rifle: int) -> void:
	$pistol_hud.set_ammo(ammo_pistol)
	$rifle_hud.set_ammo(ammo_rifle)

func select_pistol() -> void:
	$rifle_hud.disable()
	$pistol_hud.enable()

func select_rifle() -> void:
	$pistol_hud.disable()
	$rifle_hud.enable()

func hide_rifle() -> void:
	$rifle_hud.hide()

func show_rifle() -> void:
	$rifle_hud.show()
	
func show_switch_circle_interaction() -> void:
	$switch_circle.show_interaction()

func hide_switch_circle_interaction() -> void:
	$switch_circle.hide_interaction()

func show_chest_interaction() -> void:
	$open_chest.show_interaction()

func hide_chest_interaction() -> void:
	$open_chest.hide_interaction()

func show_next_height_interaction() -> void:
	$next_height.show_interaction()

func hide_next_height_interaction() -> void:
	$next_height.hide_interaction()

func show_next_level_interaction() -> void:
	$next_level.show_interaction()

func hide_next_level_interaction() -> void:
	$next_level.hide_interaction()

	
