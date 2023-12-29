extends Node

var menu_music = load("res://sound/menu_music.mp3")
var level_music = load("res://sound/level_music_2_songs.mp3")


func play_menu_music() -> void:
	if $Music.stream != menu_music:
		$Music.stream = menu_music
		$Music.play()

func play_level_music() -> void:
	if $Music.stream != level_music:
		$Music.stream = level_music
		$Music.play()
