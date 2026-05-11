import pytest
from unittest.mock import patch

from poose.onboarding.ab_03.hangman.app import app


# -----------------------------
# Flask Test Client
# -----------------------------
@pytest.fixture
def client():
    app.config['TESTING'] = True

    with app.test_client() as client:
        with app.app_context():
            yield client


# -----------------------------
# Startseite
# -----------------------------
def test_index_page(client):
    response = client.get('/')

    assert response.status_code == 200


# -----------------------------
# Neues Spiel starten
# -----------------------------
@patch('random.choice', return_value='PYTHON')
def test_start_game(mock_choice, client):
    response = client.post('/api/start')

    data = response.get_json()

    assert response.status_code == 200
    assert data['status'] == 'playing'
    assert data['mistakes'] == 0
    assert data['display_word'] == '_ _ _ _ _ _'


# -----------------------------
# Richtiger Buchstabe
# -----------------------------
@patch('random.choice', return_value='PYTHON')
def test_correct_guess(mock_choice, client):
    client.post('/api/start')

    response = client.post(
        '/api/guess',
        json={'letter': 'P'}
    )

    data = response.get_json()

    assert data['display_word'] == 'P _ _ _ _ _'
    assert data['mistakes'] == 0
    assert 'P' in data['guessed_letters']


# -----------------------------
# Falscher Buchstabe
# -----------------------------
@patch('random.choice', return_value='PYTHON')
def test_wrong_guess(mock_choice, client):
    client.post('/api/start')

    response = client.post(
        '/api/guess',
        json={'letter': 'Z'}
    )

    data = response.get_json()

    assert data['mistakes'] == 1
    assert data['status'] == 'playing'


# -----------------------------
# Doppelte Eingabe zählt nicht doppelt
# -----------------------------
@patch('random.choice', return_value='PYTHON')
def test_duplicate_guess(mock_choice, client):
    client.post('/api/start')

    client.post('/api/guess', json={'letter': 'Z'})

    response = client.post('/api/guess', json={'letter': 'Z'})

    data = response.get_json()

    assert data['mistakes'] == 1


# -----------------------------
# Spiel gewinnen
# -----------------------------
@patch('random.choice', return_value='HI')
def test_win_game(mock_choice, client):
    client.post('/api/start')

    client.post('/api/guess', json={'letter': 'H'})

    response = client.post('/api/guess', json={'letter': 'I'})

    data = response.get_json()

    assert data['status'] == 'won'
    assert data['display_word'] == 'H I'


# -----------------------------
# Spiel verlieren
# -----------------------------
@patch('random.choice', return_value='HI')
def test_lose_game(mock_choice, client):
    client.post('/api/start')

    wrong_letters = ['A', 'B', 'C', 'D', 'E', 'F']

    for letter in wrong_letters:
        response = client.post(
            '/api/guess',
            json={'letter': letter}
        )

    data = response.get_json()

    assert data['status'] == 'lost'
    assert data['mistakes'] == 6
    assert data['word'] == 'HI'


# -----------------------------
# Nach Game Over keine Änderungen mehr
# -----------------------------
@patch('random.choice', return_value='HI')
def test_no_guess_after_game_over(mock_choice, client):
    client.post('/api/start')

    wrong_letters = ['A', 'B', 'C', 'D', 'E', 'F']

    for letter in wrong_letters:
        client.post('/api/guess', json={'letter': letter})

    response = client.post(
        '/api/guess',
        json={'letter': 'H'}
    )

    data = response.get_json()

    assert data['status'] == 'lost'
    assert data['display_word'] == '_ _'