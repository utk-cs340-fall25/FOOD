import requests
import pprint
import string

try:
    alphabet = list(string.ascii_lowercase)

    for letter in alphabet:
        url = 'https://www.themealdb.com/api/json/v1/1/search.php?f='
        url = url + letter

        response = requests.get(url)
        response.raise_for_status()
        data = response.json()

        # pprint.pprint(data)

        if data['meals'] is None:
            continue
        for meal in data['meals']:
            filename = "./recipes/" + str(meal['strMeal']) + ".rcp"
            file = open(filename, "w")
            
            # name
            file.write(meal['strMeal'])
            file.write('\n')

            # ingredients
            # measure\n name\n
            for i in range(1, 21):
                ingredient = 'strIngredient' + str(i)
               # print(f"{meal[ingredient]}")
                if meal[ingredient] is not None and meal[ingredient] != "":
                    measure = 'strMeasure' + str(i)
                    file.write(meal[measure])
                    file.write('\n')
                    file.write(meal[ingredient])
                    file.write('\n')

            # instructions
            file.write("instructions")
            file.write('\n')
            file.write(meal['strInstructions'])
            file.write('\n')

            # tags
            file.write("tags")
            file.write('\n')
            file.write(meal['strArea'])
            file.write('\n')

            file.close()

except requests.exceptions.RequestException as e:
    print(f"Error fetching data: {e}")
