"""
Gemini 3.1 Pro:
Erstelle eine flask application unter ab_03 hangman,
welche ein hangman speil darstellt.
dazu soll auf der website eine viduelle darstellung eines hangmans gebaut werden,
sowei eine tastatur auf der festplatte, wo buchstaben rot werden,
wenn sie schon vorgekommen sind.
Das backend bitte mit python und rest api.

Bewertung: Spiel direkt spielbar


 Gemini 3.1 Pro:

Bitte füge dem frontend einen Tastatur listener hinzu.
Zusätzlich sollen buchstaben die drinnen waren grün werden.

Bewertung: Tastatur listener funktioniert.


Gemini 3.1 Pro:
bitte lass app.py die Wörter aus der words Datei nutzten.


"""


from flask import Flask, request, jsonify, render_template, session
import random

import os

app = Flask(__name__)
app.secret_key = 'super_secret_hangman_key'

words_file_path = os.path.join(os.path.dirname(__file__), 'words')
try:
    with open(words_file_path, 'r', encoding='utf-8') as f:
        WORDS = [line.strip().upper() for line in f if line.strip()]
    if not WORDS:
        WORDS = ["PYTHON", "FLASK", "HANGMAN", "DEVELOPER", "WEB", "CODING"]
except Exception:
    WORDS = ["PYTHON", "FLASK", "HANGMAN", "DEVELOPER", "WEB", "CODING"]

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/start', methods=['POST'])
def start():
    session['word'] = random.choice(WORDS)
    session['guessed_letters'] = []
    session['mistakes'] = 0
    return get_state()

@app.route('/api/guess', methods=['POST'])
def guess():
    data = request.json
    letter = data.get('letter', '').upper()

    if session.get('status', 'playing') != 'playing':
        return get_state()

    guessed = session.get('guessed_letters', [])
    if letter and letter not in guessed:
        guessed.append(letter)
        session['guessed_letters'] = guessed
        if letter not in session['word']:
            session['mistakes'] = session.get('mistakes', 0) + 1

    return get_state()

def get_state():
    word = session.get('word', '')
    guessed = session.get('guessed_letters', [])
    mistakes = session.get('mistakes', 0)

    display_word = ''.join([c if c in guessed else '_' for c in word])

    max_mistakes = 6
    if mistakes >= max_mistakes:
        status = 'lost'
    elif '_' not in display_word:
        status = 'won'
    else:
        status = 'playing'

    session['status'] = status

    response = {
        'display_word': display_word.replace('', ' ').strip(),
        'guessed_letters': guessed,
        'mistakes': mistakes,
        'status': status
    }
    if status == 'lost':
        response['word'] = word

    return jsonify(response)

if __name__ == '__main__':
    app.run(debug=True)

