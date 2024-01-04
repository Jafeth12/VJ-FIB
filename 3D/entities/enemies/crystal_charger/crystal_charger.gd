class_name CrystalCharger extends GenericEnemy

var bullet_cc = preload("res://entities/bullets/bullet_crystal_charger.tscn")

var cc_attack_ended: bool = false
var cc_player_in_area: bool = false
var player_node = null

func _ready():
	$sprite.play("idle")
	cc_attack_ended = false
	cc_player_in_area = false
	ENEMY_INIT_SHIELD = 200
	ENEMY_INIT_HEALTH = 90
	enemy_init_bars($SubViewport/ShieldBar3D, $SubViewport/HealthBar3D)
	$sprite.connect("animation_finished", cc_on_animation_finished)
	$cc_player_detection.connect("body_entered", cc_area_entered)
	$cc_player_detection.connect("body_exited", cc_area_exited)
	player_node = get_node("/root/main/Player")
	super()

func _process(delta):
	match enemy_state:
		EnemyState.WANDER:
			if cc_player_in_area:
				cc_shoot()
	super(delta)

# ===== REIMPLEMENTACIONES DE ENTITY =====

func entity_should_jump() -> bool:
	return false # Este personaje no salta

func entity_jump() -> float:
	return 0 # Nunca saltará

func entity_get_new_alpha(current_alpha: float, direction: EntityDirection, delta: float) -> float:
	return current_alpha # Este personaje no se mueve

func entity_get_new_direction(current_direction: EntityDirection) -> EntityDirection:
	var prx_dir: EntityDirection = current_direction
	match enemy_state:
		EnemyState.WANDER:
			if cc_player_in_area:
				prx_dir = enemy_get_direction_to_player(player_node)
		EnemyState.ATTACK:
			if cc_player_in_area:
				prx_dir = enemy_get_direction_to_player(player_node)

	$sprite.set_flip_h(current_direction==EntityDirection.RIGHT)
	return prx_dir


# ===== REIMPLEMENTACIONES DE ENEMY =====

func enemy_update_animation() -> void:
	match enemy_state:
		EnemyState.WAIT:
			if $sprite.animation != "idle":
				$sprite.play("idle")
		EnemyState.WANDER:
			if $sprite.animation != "idle":
				$sprite.play("idle")
		EnemyState.ATTACK:
			if $sprite.animation != "attack":
				$sprite.play("attack")
		EnemyState.DEAD:
			if $sprite.animation != "die":
				$sprite.play("die")

func enemy_should_attack() -> bool:
	return cc_player_in_area

func enemy_is_attack_finished() -> bool:
	var ae_copy: bool = cc_attack_ended
	cc_attack_ended = false
	return ae_copy

func cc_on_animation_finished() -> void:
	match $sprite.animation:
		"attack":
			cc_attack_ended = true

func cc_area_entered(body: Node3D):
	cc_player_in_area = true

func cc_area_exited(body: Node3D):
	cc_player_in_area = false

func cc_shoot():
	var b = bullet_cc.instantiate()
	var pos = get_position()
	b.init(pos, entity_alpha, enemy_get_direction_to_player(player_node), entity_radius, false)
	owner.add_child(b)
