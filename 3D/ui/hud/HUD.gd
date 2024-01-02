extends Control

# Called when the node enters the scene tree for the first time.
func _ready():
	set_health(100)
	hide_rifle()
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
	
	
