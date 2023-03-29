package com.example.mineswapper

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.RadioGroup
import androidx.appcompat.app.AppCompatActivity

class ChooseSize : AppCompatActivity() {

    private lateinit var boardSizeRadioGroup: RadioGroup
    private lateinit var difficultyRadioGroup: RadioGroup


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.choose_activyty)

        boardSizeRadioGroup = findViewById(R.id.board_size)
        difficultyRadioGroup = findViewById(R.id.trudnosc)

        val startGameButton: Button = findViewById(R.id.start_game_button)
        startGameButton.setOnClickListener {
            val selectedSize = when (boardSizeRadioGroup.checkedRadioButtonId) {
                R.id.radio_5 -> 5
                R.id.radio_7 -> 7
                R.id.radio_9 -> 9
                R.id.radio_12 -> 12
                else -> 9
            }

            val selectedDifficulty = when (difficultyRadioGroup.checkedRadioButtonId){
                R.id.malo -> 10
                R.id.srednio -> 20
                R.id.duzo -> 30
                else -> 0
            }
            val intent = Intent(this, MainActivity::class.java)
            intent.putExtra("board_size", selectedSize)
            intent.putExtra("difficulty", selectedDifficulty)
            startActivity(intent)
        }
    }


}
