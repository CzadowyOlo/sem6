package com.example.mineswapper

import android.app.ActionBar
import android.content.Context
import android.content.res.Resources
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.DisplayMetrics
import android.view.Gravity
import android.view.ViewGroup
import android.view.WindowManager
import android.widget.Button
import android.widget.GridLayout
import android.widget.LinearLayout
import android.widget.Toast

import android.view.View
import android.view.inputmethod.InputMethodManager
import android.widget.*
import androidx.core.content.ContextCompat
import androidx.core.content.ContextCompat.getSystemService


class MainActivity : AppCompatActivity() {

    private var game: MinesweeperGame? = null
    var buttons: Array<Button>? = null
    var flaga = false
    var boardSize = 0
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)


        boardSize = intent.getIntExtra("board_size", 0)
        val difficulty = intent.getIntExtra("difficulty", 0)
        println("\n malpa cippa \n")
        print(boardSize)
        println("\n malpa cippa \n")

        // Tworzy pusty LinearLayout
        //val layout = LinearLayout(this)

        //layout.orientation = LinearLayout.VERTICAL
        val layout = findViewById<LinearLayout>(R.id.linears)

        val boardView = createGameBoard(boardSize, this)
        layout.addView(boardView)
        //setContentView(layout)

        game = MinesweeperGame(boardSize, difficulty * boardSize * boardSize / 100, applicationContext)
        buttons = Array(boardSize * boardSize) {
            Button(this).apply {
                // ustawienia przycisku
            }
        }
        findViewById<TextView>(R.id.leftovers).text = "pozostało min: " + (game?.getRemainingMines()
            ?: 69)
        findViewById<TextView>(R.id.toogler).text = "minowanie"

    }

    fun createGameBoard(boardSize: Int, context: Context): GridLayout {
        // Uzyskaj rozmiar ekranu
        val displayMetrics = DisplayMetrics()
        val windowManager = context.getSystemService(Context.WINDOW_SERVICE) as WindowManager
        windowManager.defaultDisplay.getMetrics(displayMetrics)
        val screenWidth = displayMetrics.widthPixels
        val screenHeight = displayMetrics.heightPixels

        // Ustaw rozmiar guzika
        val buttonSize = (screenWidth * 0.95 / boardSize).toInt()

        // Utwórz GridLayout
        val gridLayout = GridLayout(this)
        gridLayout.columnCount = boardSize
        gridLayout.rowCount = boardSize
        var numid = 0;
        // Utwórz i dodaj guziki do GridLayoutu
        for (i in 0 until boardSize) {
            for (j in 0 until boardSize) {
                val button = Button(this)

                button.id = numid

                button.setOnClickListener {
                    // Wywołanie funkcji handleClick z położeniem przycisku na planszy jako argumentem
                    handleClick(button.id)
                }


                button.layoutParams = GridLayout.LayoutParams().apply {
                    width = buttonSize
                    height = buttonSize
                    setMargins(1, 1, 1, 1)
                }
                gridLayout.addView(button)
                numid++
            }
        }

        // Ustaw rozmiar GridLayoutu
        val gridLayoutParams = ViewGroup.LayoutParams(screenWidth, screenWidth)
        gridLayout.layoutParams = gridLayoutParams
        gridLayout.requestLayout()

        return gridLayout
    }

    private fun handleClick(numid: Int) {

        //var huj = this.getId()
        println("\n malpa cippa \n")
        print(numid)
        println("\n malpa cippa \n")
        val button = findViewById<Button>(numid)
        if(flaga){
            game?.toggleFlag(numid, this, this)
            if (game?.isGameWon() == true) {
                // Wyświetl komunikat o wygranej
                Toast.makeText(this, "You won!", Toast.LENGTH_LONG).show()
                game?.reset()
                for(i in 0 until  boardSize * boardSize){
                    val button = findViewById<Button>(i)
                    button.setBackgroundResource(android.R.drawable.btn_default)
                }
            }
        }
        else{
            if (game?.isMine(numid) == true) {
                // Przycisk zawiera minę
                button.setBackgroundResource(R.drawable.mine)
                // setBackgroundResource(R.drawable.mine)
                //game.reveal()
                // Wyświetl komunikat o przegranej
                Toast.makeText(this, "You lost!", Toast.LENGTH_LONG).show()
                //game?.reset()
                for(i in 0 until  boardSize * boardSize){
                    val button = findViewById<Button>(i)
                    if(game?.isMine(i)!!){
                        button.setBackgroundResource(R.drawable.mine)
                    }
                    //button.setBackgroundResource(android.R.drawable.btn_default)
                }
            } else {
                // Przycisk nie zawiera miny
                println("\n nie ma miny \n")
                game?.reveal(numid, this, this)
                // Zaktualizuj ikonę przycisku na planszy
                //updateButton(numid)
                if (game?.isGameWon() == true) {
                    // Wyświetl komunikat o wygranej
                    Toast.makeText(this, "You won!", Toast.LENGTH_LONG).show()
                    game?.reset()
                    for(i in 0 until  boardSize * boardSize){
                        val button = findViewById<Button>(i)
                        button.setBackgroundResource(android.R.drawable.btn_default)
                    }
                }
            }
        }
        findViewById<TextView>(R.id.leftovers).text = "pozostało min: " + (game?.getRemainingMines()
            ?: 69)

    }

    fun reseter(view: View) {
        game?.reset()
        for(i in 0 until  boardSize * boardSize){
            val button = findViewById<Button>(i)
            button.setBackgroundResource(android.R.drawable.btn_default)
        }
    }

    fun flaga(view: View) {
        if(flaga){
            findViewById<TextView>(R.id.toogler).text = "minowanie"
        }
        else{
            findViewById<TextView>(R.id.toogler).text = "flagowanie"

        }
        flaga = !flaga
    }


}
class MinesweeperGame(private val boardSize: Int, private val numMines: Int, val context: Context) {
    private val resources = context.resources
    private val packageName = context.packageName
    private val mineLocations = HashSet<Int>()
    private val revealed = HashSet<Int>()
    private val flagged = HashSet<Int>()

    init {
        placeMines()
    }

    private fun placeMines() {
        var minesToPlace = numMines
        while (minesToPlace > 0) {
            val location = (Math.random() * (boardSize * boardSize)).toInt()
            if (!mineLocations.contains(location)) {
                mineLocations.add(location)
                minesToPlace--
            }
        }
    }

    fun countAdjacentMines(location: Int): Int {
        var count = 0
        val row = location / boardSize
        val col = location % boardSize
        for (i in -1..1) {
            for (j in -1..1) {
                // pomijamy iteracje dla pola samo dla siebie oraz dla brzegowych pól
                if ((i != 0 || j != 0) && row + i >= 0 && row + i < boardSize && col + j >= 0 && col + j < boardSize) {
                    val adjLocation = (row + i) * boardSize + col + j
                    if (mineLocations.contains(adjLocation)) {
                        count++
                    }
                }
            }
        }
        return count
    }

    fun isMine(location: Int): Boolean {
        return mineLocations.contains(location)
    }

    fun isRevealed(location: Int): Boolean {
        return revealed.contains(location)
    }

    fun isFlagged(location: Int): Boolean {
        return flagged.contains(location)
    }

    fun getNumAdjacentMines(location: Int): Int {
        return countAdjacentMines(location)
    }

    fun reveal(location: Int, context: Context, activity: AppCompatActivity): Boolean {
        val button = activity.findViewById<Button>(location)
        var resID = 0;
        if (isRevealed(location) || isFlagged(location)) {
            return false
        }
        revealed.add(location)
        if (isMine(location)) {
            return true
        }
        var param = getNumAdjacentMines(location)
        if (param == 0) {
            button.setBackgroundResource(R.drawable.ic_empty)
            val row = location / boardSize
            val col = location % boardSize
            for (i in -1..1) {
                for (j in -1..1) {
                    // pomijamy iteracje dla pola samo dla siebie oraz dla brzegowych pól
                    if ((i != 0 || j != 0) && row + i >= 0 && row + i < boardSize && col + j >= 0 && col + j < boardSize) {
                        val adjLocation = (row + i) * boardSize + col + j
                        reveal(adjLocation, context, activity)
                    }
                }
            }
        }
        else{
            println("malpa cippa podaje wynik saisadow")
            print(param)
            println("malpa cippa podaje wynik saisadow")
            resID = resources.getIdentifier("part$param", "drawable", packageName)
            if(location in 0 until boardSize * boardSize){
                button.setBackgroundResource(resID)

            }
        }
        return false
    }

    fun toggleFlag(location: Int, context: Context, activity: AppCompatActivity): Boolean {
        if (isRevealed(location)) {
            return false
        }
        val button = activity.findViewById<Button>(location)
        if (isFlagged(location)) {
            flagged.remove(location)
            button.setBackgroundResource(android.R.drawable.btn_default)

        } else {
            flagged.add(location)
            button.setBackgroundResource(R.drawable.flag)
        }
        return true
    }

    fun isGameWon(): Boolean {
        return revealed.size == boardSize * boardSize - numMines &&
                flagged.containsAll(mineLocations)
    }

    fun getRemainingMines(): Int {
        return numMines - flagged.size
    }

    fun reset() {
        mineLocations.clear()
        revealed.clear()
        flagged.clear()
        placeMines()
    }
}
