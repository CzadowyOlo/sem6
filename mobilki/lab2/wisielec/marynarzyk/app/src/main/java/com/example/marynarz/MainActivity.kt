package com.example.marynarz

import android.content.Context
import android.media.MediaPlayer
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.view.inputmethod.InputMethodManager
import android.widget.*
//import com.google.gson.Gson
//import com.google.gson.reflect.TypeToken


import kotlin.random.Random

class MainActivity : AppCompatActivity() {

    private var word = ""
    private var dupa = false;
    private var cyce = true;
    private var letter = '*'
    private val dynamicArray = mutableListOf<Int>()
    private val indexes = mutableListOf<Int>()
    private val displayWord = StringBuilder()
    private var guessedLetters = 0
    private var guessedWord = "*"
    private var score = 0;
    var soundID = 0
    private var missses = 0;
    private lateinit var mediaPlayer: MediaPlayer
    //private var mediaPlayer = MediaPlayer.create(this, R.raw.phase0)

    //private
    private val categories = listOf(
        "fruits",
        "vegetables",
        "IT",
        "minecraft",
        "countries"
    )
    private val wordsList = listOf(
        listOf("apple", "banana", "orange", "pineapple", "pear",
            "kiwi", "strawberry", "grape", "watermelon", "melon",
            "cherry", "peach", "apricot", "plum", "blueberry",
            "raspberry", "blackberry", "lemon", "lime", "grapefruit",
            "mango", "pomegranate", "fig", "avocado", "coconut",
            "papaya", "pepino", "persimmon", "guava", "lychee"),
        listOf("artichoke", "asparagus", "beetroot", "broccoli", "cabbage",
            "carrot", "cauliflower", "celery", "corn", "cucumber",
            "eggplant", "garlic", "ginger", "kale", "leek",
            "lettuce", "mushroom", "okra", "onion", "pepper",
            "potato", "pumpkin", "radish", "spinach", "squash",
            "sweet potato", "tomato", "turnip", "zucchini", "watercress"),
        listOf("algorithm", "api", "array", "boolean", "class",
            "compiler", "data type", "debugging", "encryption", "function",
            "gui", "ide", "inheritance", "loop", "method",
            "object", "operator", "pointer", "polymorphism", "recursion",
            "runtime", "scripting language", "source code", "string", "gebala",
            "testing", "variable", "web development", "xml", "shortest path"),
        listOf("creeper", "diamond", "enderman", "ghast", "gold ingot", "iron golem", "jungle temple", "lava",
            "nether fortress", "obsidian", "phantom", "potion", "redstone", "spider", "stronghold", "slender man", "villager",
            "modpack", "zombie pigman", "blaze",
            "cave spider", "drowned", "enchanted book", "fire charge", "guardians", "magma cube", "nether portal",
            "steve", "cave", "shulker box"),
        listOf( "ukraine", "switzerland", "netherlands", "colombia", "niger",
            "slovakia", "philippines", "tanzania", "south korea", "japan",
            "mexico", "thailand", "vietnam", "indonesia", "sweden",
            "portugal", "india", "malaysia", "iran", "egypt",
            "france", "argentina", "pakistan", "canada", "brazil",
            "united kingdom", "spain", "italy", "united states", "germany")
    )


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mediaPlayer = MediaPlayer.create(this, R.raw.phase0)
        setContentView(R.layout.activity_main)
        generateWord()
        resetDisplay()
    }

    override fun onDestroy() {
        super.onDestroy()
        mediaPlayer.stop()
        mediaPlayer.release()
    }

    private fun hideKeyboard() {
        val imm = getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
        imm.hideSoftInputFromWindow(currentFocus?.windowToken, 0)
    }


    private fun generateWord() {
        val randomCategoryIndex = (categories.indices).random()
        val randomWordIndex = (0 until 30).random()
        val randomCategory = categories[randomCategoryIndex]
        word = wordsList[randomCategoryIndex][randomWordIndex]
        "category:  \n$randomCategory  ".also { findViewById<TextView>(R.id.category).text = it };
        println("WORD" + word)
        for(i in word.indices){
            indexes.add(i)
        }
    }
    private fun resetDisplay(){
        displayWord.clear()
        for (i in word.indices){
            if(word[i] == ' '){
                displayWord.append("  ")
            }
            else{
                displayWord.append("*")
            }
        }
        findViewById<TextView>(R.id.guessWord).text = displayWord.toString()
        updateScore(0)
    }

    private fun getGuessedLettersCount(guessedWord: String) {
        dupa = false;
        cyce = true;
        for (i in word.indices) {
            if (guessedWord[0] == word[i]) {
                for (elem in dynamicArray){
                    if (elem == i){
                        Toast.makeText(this, "this letter was already guessed!", Toast.LENGTH_SHORT).show()
                        dupa = true;
                        break
                    }
                }
                if(!dupa){
                    cyce = false;
                    dynamicArray.add(i)
                    guessedLetters++
                }

            }
        }
        if(cyce){
            missses++
            hideKeyboard()
            updateScore(missses)
        }
    }

    fun updateScore (misses: Int){
        val imagedeath: ImageView = findViewById<ImageView>(R.id.szubienica)



        var resID = 0
        if(misses in 0..9){
            resID = resources.getIdentifier("part$misses", "drawable", packageName)
            soundID = resources.getIdentifier("phase$misses", "raw", packageName)
            mediaPlayer.stop() // Zatrzymanie bieżącego odtwarzanego dźwięku
            mediaPlayer.release()
            mediaPlayer = MediaPlayer.create(this, soundID)
            mediaPlayer.start() // Rozpoczęcie odtwarzania nowego dźwięku
        } else if (misses >= 10){
            soundID = resources.getIdentifier("phase$misses", "raw", packageName)
            mediaPlayer.stop() // Zatrzymanie bieżącego odtwarzanego dźwięku
            mediaPlayer.release()
            mediaPlayer = MediaPlayer.create(this, soundID)
            mediaPlayer.start() // Rozpoczęcie odtwarzania nowego dźwięku
            resID = resources.getIdentifier("part10", "drawable", packageName)
            Toast.makeText(this, "BAD ENDING!", Toast.LENGTH_LONG).show()
            findViewById<TextView>(R.id.winner).text = "YOU LOSE!"
            findViewById<Button>(R.id.WordButton).isEnabled = false
            findViewById<Button>(R.id.LetterButton).isEnabled = false
        }
        else{
            resID = resources.getIdentifier("part$misses", "drawable", packageName)
            mediaPlayer.stop()
            mediaPlayer.release()
            mediaPlayer = MediaPlayer.create(this, R.raw.winner)
            mediaPlayer.isLooping = true
            mediaPlayer.start()
        }

        imagedeath.setImageResource(resID)
    }

    private fun updateDisplay() {
        for (i in dynamicArray) {
            displayWord[i] = word[i];
        }
        findViewById<TextView>(R.id.guessWord).text = displayWord.toString()
        findViewById<EditText>(R.id.guessInput).setText("")
    }

    fun guessletter(view: View) {
            score++
            guessedWord = findViewById<EditText>(R.id.guessInput).text.toString()
            if (!guessedWord.isBlank()) {
                getGuessedLettersCount(guessedWord) //indeks zgadnięty
            } else {
                Toast.makeText(this, "ENTER A LETTER!", Toast.LENGTH_SHORT).show()
            }
            updateDisplay()
            if (guessedLetters == word.length) {
                // User has guessed the whole word
                Toast.makeText(this, "GOOD!", Toast.LENGTH_SHORT).show()
                updateScore(-1)
                hideKeyboard()
                findViewById<TextView>(R.id.winner).text = "You win!\nscore: " + score
                findViewById<Button>(R.id.WordButton).isEnabled = false
                findViewById<Button>(R.id.LetterButton).isEnabled = false

            }

    }
    fun guessword(view: View) {
        score +=1;
        guessedWord = findViewById<EditText>(R.id.guessInput).text.toString()
        if (!guessedWord.isBlank()) {
            if(guessedWord == word){
                findViewById<TextView>(R.id.guessWord).text = word
                Toast.makeText(this, "GOOD!", Toast.LENGTH_SHORT).show()
                updateScore(-1)
                hideKeyboard()
                findViewById<TextView>(R.id.winner).text = "You win!\nscore: " + score
                findViewById<Button>(R.id.WordButton).isEnabled = false
                findViewById<Button>(R.id.LetterButton).isEnabled = false
            }
            else{
                Toast.makeText(this, "WRONG!", Toast.LENGTH_SHORT).show()
                findViewById<EditText>(R.id.guessInput).setText("")
                score+=2;
                missses+=2;
                hideKeyboard();
                updateScore(missses);
            }
        } else {
            Toast.makeText(this, "ENTER WORD", Toast.LENGTH_SHORT).show()
        }

    }
    fun reset(view: View) {
            generateWord()
            mediaPlayer.stop()
            mediaPlayer.release()
            mediaPlayer = MediaPlayer.create(this, R.raw.phase0)
            mediaPlayer.isLooping = true
            mediaPlayer.start()
            guessedLetters = 0
            score = 0;
            missses = 0;
            dynamicArray.clear()
            indexes.clear()
            findViewById<EditText>(R.id.guessInput).text.clear()
            findViewById<TextView>(R.id.winner).text= ""
            findViewById<Button>(R.id.WordButton).isEnabled = true
            findViewById<Button>(R.id.LetterButton).isEnabled = true

        resetDisplay()
        }

}
