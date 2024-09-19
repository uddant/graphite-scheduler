# Create and open a new text file in write mode
import random
import string


def rand_name():
    random_string = ''.join(random.choices(string.ascii_uppercase, k=6))
    return random_string


def military_time_points():
    time_points = []
    for i in range(24):
        for j in range(6):
            for k in range(10):
                time_pt = str(i)+str(j)+str(k)
                time_pt = "0"+time_pt if i<10 else time_pt
                time_points.append(time_pt)
    return time_points

def rand_time_intervals():
    number_of_slots = random.randint(1, 5)
    times = military_time_points()
    interval_markers = random.sample(times[360:1200], number_of_slots*2)
    interval_markers.sort()
    intervals = ' '.join(pt for pt in interval_markers)
    intervals = str(number_of_slots) + " " + intervals
    return intervals


def rand_state():
    states = ('AL', 'AK', 'AZ', 'AR', 'CA', 'CO', 'CT', 'DE', 'FL', 'GA', 'HI', 'ID', 'IL', 'IN', 'IA', 'KS', 'KY', 'LA', 'ME', 'MD', 'MA', 'MI', 'MN', 'MS', 'MO', 'MT', 'NE', 'NV', 'NH', 'NJ', 'NM', 'NY', 'NC', 'ND', 'OH', 'OK', 'OR', 'PA', 'RI', 'SC', 'SD', 'TN', 'TX', 'UT', 'VT', 'VA', 'WA', 'WV', 'WI', 'WY')
    return states[random.randint(0, 49)]

def generate_team_member(file, seniority=0):
    file.write(rand_name())
    file.write("\n")
    file.write(rand_state())
    file.write("\n")
    file.write(rand_time_intervals())
    file.write("\n")
    if seniority:
        file.write(str(seniority))
    else:
        file.write(str(random.randint(1, 2)))
    file.write("\n")


with open("test_data.txt", "w") as file:
    # Write the line number on the first 10 lines
    N = 10
    file.write(str(N))
    file.write("\n")
    for i in range(N):
        generate_team_member(file)

print("File created and written successfully.")