import requests
import pprint

try:
    # getting the ingredients list & making sure its good
    response = requests.get('https://www.themealdb.com/api/json/v1/1/list.php?i=list')
    response.raise_for_status()
    data = response.json()

    # checking formatting
    # pprint.pprint(data)

    # constructing the file
    ifile = open("ingredients.txt", "w")

    for ingredient in data['meals']:
        ifile.write(f"0 {ingredient['strIngredient']}\n")

    ifile.close()

except requests.exceptions.RequestException as e:
    print(f"Error fetching data: {e}")
