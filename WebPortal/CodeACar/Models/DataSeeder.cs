using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;

namespace CodeACar.Models
{
    public static class DataSeeder
    {
        public static void Initialise(IServiceProvider serverProvider)
        {
            using (var context = new ApplicationDbContext(serverProvider.GetRequiredService<DbContextOptions<ApplicationDbContext>>()))
            {
                
            }
        }
    }
}
