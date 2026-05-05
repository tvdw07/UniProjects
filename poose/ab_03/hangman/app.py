from flask import Flask, request, jsonify, render_template, session
import random

app = Flask(__name__)
app.secret_key = 'super_secret_hangman_key'

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

